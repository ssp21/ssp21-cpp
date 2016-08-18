/**
 * License TBD
 */
package com.automatak.render.ssp21.enums.generators

import _root_.java.nio.file.Path

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21._

case class EnumGenerator(enums: List[EnumConfig], cppNamespace : String) {

  private def renderers(cfg: EnumConfig): List[HeaderImplModelRender[EnumModel]] = {

    def conversions = if (cfg.conversions) List(EnumToType, EnumFromType) else Nil
    def stringify = if (cfg.stringConv) List(EnumToString) else Nil

    conversions ::: stringify
  }

  private def headerName(model: EnumModel) = model.name + ".h"
  private def implName(model: EnumModel) = model.name + ".cpp"

  def write(incDirectory: Path, implDirectory: Path): Unit = {

    implicit val indent = CppIndentation()

    def headerPath(model: EnumModel) = incDirectory.resolve(headerName(model))
    def implPath(model: EnumModel) = implDirectory.resolve(implName(model))

    def writeHeader(e: EnumConfig): Unit =  {
      writeTo(headerPath(e.model))(header(e))
      println("Wrote: " + headerPath(e.model))
    }

    def writeImpl(e: EnumConfig): Unit =  {
      if(e.anyOptionalFunctions) {
        writeTo(implPath(e.model))(impl(e))
        println("Wrote: " + implPath(e.model))
      }
    }

    enums.foreach { e =>
      writeHeader(e)
      writeImpl(e)
    }
  }

  private def header(cfg: EnumConfig)(implicit i : Indentation) : Iterator[String] = {

    def license = commented(LicenseHeader())
    def enum = EnumModelRenderer.render(cfg.model)
    def signatures = renderers(cfg).map(c => c.header.render(cfg.model)).flatten.toIterator
    def spec = "struct %s : private openpal::StaticOnly".format(cfg.model.specName).iter ++ bracketSemiColon {
      "typedef %s enum_type_t;".format(cfg.model.name).iter ++
        space ++
        signatures
    }

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

    def includes : List[Include] = List(Includes.uncopyable, Includes.cstdint)

    license ++ space ++ includeGuards(cfg.model.name) (
      Includes.lines(includes) ++ space ++ namespace(cppNamespace)(
        enum ++ castFunc ++ space ++ spec
      )
    )
  }

  private def impl(cfg: EnumConfig)(implicit i : Indentation) : Iterator[String] = {

    def license = commented(LicenseHeader())
    def funcs = renderers(cfg).map(r => r.impl.render(cfg.model)).flatten.toIterator
    def inc = quoted("ssp21/gen/%s".format(headerName(cfg.model)))

    license ++ space ++ include(inc) ++ space ++ namespace(cppNamespace)(funcs)
  }
}
