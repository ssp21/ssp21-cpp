package com.automatak.render.ssp21.messages.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21.{Includes, WriteCppFiles}
import com.automatak.render.ssp21.messages.{Bit, Bitfield}

case class BitfieldStructGenerator(field: Bitfield) extends WriteCppFiles {

  def mainClassName : String = field.structName
  def license = commented(LicenseHeader.lines)

  private def cppNamespace = "ssp21"

  def header(implicit indent: Indentation) : Iterator[String] = {

    def members : Iterator[String] = field.bits.map(b => "bool %s = %b;".format(b.name, b.default)).toIterator

    def defaultConstructor = "%s(){}".format(field.structName).iter

    def fullConstructor = {
      def decl(b: Bit) = "bool %s".format(b.name)

      def init = commas(field.bits.map(b => "%s(%s)".format(b.name, b.name)))

      "%s(%s) :".format(field.structName, commas(field.bits.map(decl)).mkString(" ")).iter ++ indent(init) ++ "{}".iter
      
    }

    def readFunc : Iterator[String] = {
      "virtual ParseError read(openpal::RSlice& input) override;".iter
    }

    def writeFunc : Iterator[String] = {
      "virtual FormatError write(openpal::WSlice& output) override;".iter
    }

    def struct = "struct %s final : public IReadable, public IWritable".format(field.structName).iter ++ bracketSemiColon {
      defaultConstructor ++
      space ++
      fullConstructor ++
      space ++
      readFunc ++
      writeFunc ++
      space ++
      members
    }

    def includes = List(Includes.readable, Includes.writable)
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

    def writeFunc : Iterator[String] = {

      def fields : Iterator[String] = field.bits.zipWithIndex.map { case (b, i) =>
        val mask = "0x%02X".format(1 << (7-i))
        "if(%s) value |= %s;".format(b.name, mask)
      }.toIterator

      "FormatError %s::write(openpal::WSlice& output)".format(field.structName).iter ++ bracket {
        "if(output.is_empty()) return FormatError::insufficient_space;".iter ++ space ++
          "uint8_t value = 0;".iter ++
          space ++ fields ++ space ++
          "output[0] = value;".iter ++
          "output.advance(1);".iter ++
          space ++
          "return FormatError::ok;".iter
      }
    }

    def selfInclude = include(quoted("ssp21/msg/%s".format(headerFileName)))
    def content = readFunc ++ space ++ writeFunc


    license ++ space ++ selfInclude ++ space ++ namespace(cppNamespace) {
      content
    }
  }

}
