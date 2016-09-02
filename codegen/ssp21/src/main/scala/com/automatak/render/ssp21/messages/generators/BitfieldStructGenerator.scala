package com.automatak.render.ssp21.messages.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21.messages.{Bit, Bitfield}
import com.automatak.render.ssp21.{Includes, WriteCppFiles}

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
      "virtual FormatError write(openpal::WSlice& output) const override;".iter
    }

    def printFunc : Iterator[String] = {
      "virtual void print(const char* name, IMessagePrinter& printer) const override;".iter
    }

    def struct = "struct %s final : public IReadable, public IWritable, public IPrintable".format(field.structName).iter ++ bracketSemiColon {
      defaultConstructor ++
      space ++
      fullConstructor ++
      space ++
      readFunc ++
      writeFunc ++
      printFunc ++
      space ++
      members
    }

    def includes = List(Includes.readable, Includes.writable, Includes.msgPrinter)
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

      def reserved_mask : String = {
        val valid_mask = field.bits.zipWithIndex.map { case (b, i) => (1 << (7-i)) }.foldLeft(0)((bit, sum) => bit | sum)
        "0x%02X".format(~valid_mask.toByte)
      }

      "ParseError %s::read(openpal::RSlice& input)".format(field.structName).iter ++ bracket {
        "uint8_t value = 0;".iter ++
        "if(!openpal::UInt8::read_from(input, value)) return ParseError::insufficient_bytes;".iter ++ space ++
        "if((value & %s) != 0) return ParseError::reserved_bit;".format(reserved_mask).iter ++ space ++
          fields ++ space ++
          "return ParseError::ok;".iter
      }
    }

    def writeFunc : Iterator[String] = {

      def fields : Iterator[String] = field.bits.zipWithIndex.map { case (b, i) =>
        val mask = "0x%02X".format(1 << (7-i))
        "if(%s) value |= %s;".format(b.name, mask)
      }.toIterator

      "FormatError %s::write(openpal::WSlice& output) const".format(field.structName).iter ++ bracket {
          "uint8_t value = 0;".iter ++
          space ++ fields ++ space ++
          "return openpal::UInt8::write_to(output, value) ? FormatError::ok : FormatError::insufficient_space;".iter
      }
    }

    def printFunc : Iterator[String] = {
      // FlagsPrinting::print(printer, name, "fir", fir, "fin", fin);

      def fields : String = field.bits.map(b => "%s, %s".format(quoted(b.name), b.name)).mkString(", ")

      "void %s::print(const char* name, IMessagePrinter& printer) const".format(field.structName).iter ++ bracket {
        "FlagsPrinting::print(printer, name, %s);".format(fields).iter
      }
    }

    def selfInclude = include(quoted("ssp21/msg/%s".format(headerFileName)))

    def otherIncludes = Includes.lines(List(Includes.bigEndian, Includes.flagsPrinting))

    def content = readFunc ++ space ++ writeFunc ++ space ++ printFunc


    license ++ space ++ selfInclude ++ space ++ otherIncludes ++ space ++ namespace(cppNamespace) {
      content
    }
  }

}
