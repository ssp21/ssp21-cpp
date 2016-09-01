package com.automatak.render.ssp21.messages.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21.{Includes, WriteCppFiles}
import com.automatak.render.ssp21.messages.Bitfield

case class BitfieldStructGenerator(field: Bitfield) extends WriteCppFiles {

  def mainClassName : String = field.structName
  def license = commented(LicenseHeader.lines)

  private def cppNamespace = "ssp21"

  def header(implicit indent: Indentation) : Iterator[String] = {

    def members : Iterator[String] = field.bits.map(b => "bool %s = %b;".format(b.name, b.default)).toIterator
    def defaultConstructor = "%s(){}".format(field.structName).iter

    def readFunc : Iterator[String] = {
      "virtual ParseError read(openpal::RSlice& input) override;".iter
    }

    def struct = "struct %s final : public IReadable".format(field.structName).iter ++ bracketSemiColon {
      defaultConstructor ++
      space ++
      readFunc ++
      space ++
      members
    }

    def includes = List(Includes.readable)
    def includeLines : Iterator[String] = Includes.lines(includes)

    def content = struct

    license ++ space ++ includeGuards(field.structName) {
      includeLines ++
      space ++
      namespace(cppNamespace)(content)
    }

  }

  def impl(implicit i : Indentation) : Iterator[String] = {

    def readFunc : Iterator[String] = {

      def fields : Iterator[String] = field.bits.zipWithIndex.map { case (b, i) =>
        val mask = 1 << (7-i)
        "%s = (value & 0x%02X) != 0;".format(b.name, mask)
      }.toIterator

      "ParseError %s::read(openpal::RSlice& input)".format(field.structName).iter ++ bracket {
        "if(input.is_empty()) return ParseError::insufficient_bytes;".iter ++ space ++
          Iterator("const auto value = input[0];", "input.advance(1);") ++ space ++
          fields ++ space ++
          "return ParseError::ok;".iter
      }
    }

    def selfInclude = include(quoted("ssp21/msg/%s".format(headerFileName)))
    def content = readFunc


    license ++ space ++ selfInclude ++ space ++ namespace(cppNamespace) {
      content
    }
  }

}
