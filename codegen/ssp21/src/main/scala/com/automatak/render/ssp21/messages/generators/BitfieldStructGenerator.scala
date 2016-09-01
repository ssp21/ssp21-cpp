package com.automatak.render.ssp21.messages.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21.WriteCppFiles
import com.automatak.render.ssp21.messages.Bitfield

case class BitfieldStructGenerator(field: Bitfield) extends WriteCppFiles {

  def mainClassName : String = field.structName

  private def cppNamespace = "ssp21"

  def impl(implicit i : Indentation) : Iterator[String] = Iterator.empty

  def header(implicit indent: Indentation) : Iterator[String] = {

    def members : Iterator[String] = field.bits.map(b => "bool %s = %b;".format(b.name, b.default)).toIterator
    def defaultConstructor = "%s(){}".format(field.structName).iter


    def struct = "struct %s".format(field.structName).iter ++ bracketSemiColon {
      defaultConstructor ++
      space ++
      members
    }

    def license = commented(LicenseHeader.lines)
    def content = struct

    license ++ space ++ includeGuards(field.structName)(namespace(cppNamespace)(content))
  }

}
