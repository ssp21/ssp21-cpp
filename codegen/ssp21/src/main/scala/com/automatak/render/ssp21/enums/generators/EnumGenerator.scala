/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.generators


import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21._

case class EnumGenerator(cfg: EnumConfig) extends WriteCppFiles {

  def cppNamespace = "ssp21"

  override def hasImpl: Boolean = cfg.anyOptionalFunctions

  private val renderers: List[HeaderImplModelRender[EnumModel]] = {

    def conversions = if (cfg.conversions) List(EnumToType, EnumFromType) else Nil
    def stringify = if (cfg.stringConv) List(EnumToString) else Nil

    conversions ::: stringify
  }

  override def mainClassName: String = cfg.model.name

  override def header(implicit i: Indentation): Iterator[String] = {

    def license = commented(LicenseHeader.lines)
    def enum = EnumModelRenderer.render(cfg.model)
    def signatures = renderers.map(c => c.header.render(cfg.model)).flatten.toIterator
    def spec = "struct %s : private openpal::StaticOnly".format(cfg.model.specName).iter ++ bracketSemiColon {
      "typedef %s enum_type_t;".format(cfg.model.name).iter ++
        space ++
        signatures
    }

    def castFunc: Iterator[String] = {
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

    def includes: List[Include] = List(Includes.uncopyable, Includes.cstdint)

    license ++ space ++ includeGuards(cfg.model.name)(
      Includes.lines(includes) ++ space ++ namespace(cppNamespace)(
        enum ++ castFunc ++ space ++ spec
      )
    )
  }

  override def impl(implicit i: Indentation): Iterator[String] = {

    def license = commented(LicenseHeader.lines)
    def funcs = renderers.map(r => r.impl.render(cfg.model)).flatten.toIterator
    def inc = quoted("ssp21/gen/%s".format(headerFileName))

    license ++ space ++ include(inc) ++ space ++ namespace(cppNamespace)(funcs)
  }
}
