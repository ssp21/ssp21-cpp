/**
 * License TBD
 */
package com.automatak.render.ssp21.enums.generators

import _root_.java.nio.file.Path

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21._

object EnumGenerator {

  def apply(enums: List[EnumConfig], cppNamespace : String, incFormatString: String, incDirectory: Path, implDirectory: Path): Unit = {

    implicit val indent = CppIndentation()

    def headerPath(model: EnumModel) = incDirectory.resolve(headerName(model))
    def implPath(model: EnumModel) = implDirectory.resolve(implName(model))

    def headerName(model: EnumModel) = model.name + ".h"
    def implName(model: EnumModel) = model.name + ".cpp"

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      val conversions = if(cfg.conversions) List(EnumToType, EnumFromType) else Nil
      val stringify = if(cfg.stringConv) List(EnumToString) else Nil

      val renders = conversions ::: stringify

      def writeHeader() {
        def license = commented(LicenseHeader())
        def enum = EnumModelRenderer.render(cfg.model)
        def spec = "struct %s : private openpal::StaticOnly".format(cfg.model.specName).iter ++ bracketSemiColon {
          "typedef %s enum_type_t;".format(cfg.model.name).iter ++
          space ++
          signatures
        }
        def signatures = renders.map(c => c.header.render(cfg.model)).flatten.toIterator

        def castFunc : Iterator[String] = {
          cfg.model.boolCastValue match {
            case Some(value) => {
              space ++
              "inline bool any(%s value)".format(cfg.model.name).iter ++ bracket {
                "return value != %s::%s;".format(cfg.model.name, value.name).iter
              }
            }
            case None => Iterator.empty
          }
        }

        def includes : List[Include] = List(Includes.uncopyable)

        def lines = license ++ space ++ includeGuards(cfg.model.name) (
          Includes.lines(includes) ++ cstdint ++ space ++ namespace(cppNamespace)(
            enum ++ castFunc ++ space ++ spec
          )
        )

        writeTo(headerPath(cfg.model))(lines)
        println("Wrote: " + headerPath(cfg.model))
      }

      def writeImpl() {
        def license = commented(LicenseHeader())
        def funcs = renders.map(r => r.impl.render(cfg.model)).flatten.toIterator
        def inc = quoted(String.format(incFormatString, headerName(cfg.model)))
        def lines = license ++ space ++ include(inc).iter ++ space ++ namespace(cppNamespace)(funcs)

        if(cfg.anyOptionalFunctions)
        {
          writeTo(implPath(cfg.model))(lines)
          println("Wrote: " + implPath(cfg.model))
        }
      }

      writeHeader()
      writeImpl()
    }

    enums.foreach { e =>
      writeEnumToFiles(e)
    }
  }
}
