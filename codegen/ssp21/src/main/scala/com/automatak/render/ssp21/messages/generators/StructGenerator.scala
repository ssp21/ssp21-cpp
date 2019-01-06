/**
 * License TBD
 */
package com.automatak.render.ssp21.messages.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21.messages.Struct
import com.automatak.render.ssp21.{Include, Includes, WriteCppFiles}

object StructGenerator {
  def apply(sf: Struct, public: Boolean) = new StructGenerator(sf, public)
}

class StructGenerator(sf: Struct, public: Boolean) extends WriteCppFiles {

  protected def prefixSize : Int = 0

  private def cppNamespace = "ssp21"

  private def headerName = sf.name + ".h"

  private def implName = sf.name + ".cpp"

  final override def mainClassName: String = sf.name

  /// These can be overriden in super classes
  def extraHeaderDeclarations: Iterator[String] = Iterator.empty

  def extraHeaderSignatures: Iterator[String] = Iterator.empty

  def extraHeaderConstants: Iterator[String] = Iterator.empty

  def extraImplFunctions(implicit indent: Indentation): Iterator[String] = Iterator.empty

  def interfaces: String = ""

  def headerIncludes: List[Include] = Includes.messageField

  def outputReadWritePrint: Boolean = true

  final override def header(implicit indent: Indentation): Iterator[String] = {

    def defaultConstructorSig = "%s();".format(sf.name).iter

    def readSomeSig = "ParseError read(seq32_t& input);".iter
    def readAllSig = "ParseError read_all(const seq32_t& input);".iter
    def writeSig = "FormatError write(wseq32_t& output) const;".iter
    def printSig = "void print(const char* name, IMessagePrinter& printer) const;".iter
    def sizeSig = "size_t size() const;".iter

    def readWritePrint = {
      if (outputReadWritePrint) readSomeSig ++ readAllSig ++ writeSig ++ printSig else Iterator.empty
    }

    def getMinRepresentation(num: Int): String = num match {
      case num if (num < 256) => "uint8_t"
      case num if (num < 65536) => "uint16_t"
      case _ => "uint32_t"

    }

    def sizeBytes = sf.fixedSize match {
      case Some(size) => "static const %s fixed_size_bytes = %s;".format(getMinRepresentation(size), size).iter
      case None => "static const %s min_size_bytes = %s;".format(getMinRepresentation(sf.minSizeBytes), sf.minSizeBytes).iter
    }

    def constants = sizeBytes ++ extraHeaderConstants

    def struct(implicit indent: Indentation): Iterator[String] = {

      def fieldDefintions: Iterator[String] = sf.fields.map { f =>
        "%s %s;".format(f.cpp.cppType, f.name);
      }.toIterator

      "struct %s final %s".format(sf.name, interfaces).iter ++ bracketSemiColon {
        extraHeaderDeclarations ++
          defaultConstructorSig ++
          space ++
          fullConstructorSig(false) ++
          space ++
          sizeSig ++
          space ++
          extraHeaderSignatures ++
          constants ++
          space ++
          fieldDefintions ++
          space ++
          readWritePrint
      }
    }

    def includes: Iterator[String] = {
      Includes.lines(headerIncludes ::: sf.fields.flatMap(f => f.cpp.includes.toList))
    }

    def license = commented(LicenseHeader.lines)
    def content = struct

    license ++ space ++ includeGuards(sf.name)(includes ++ space ++ namespace(cppNamespace)(content))
  }

  protected def writeInternals(implicit indent: Indentation): Iterator[String] = {

    def args = commas(sf.fields.map(_.name))

    "return MessageFormatter::write_fields(".iter ++ indent {
      "output,".iter ++ args
    } ++ ");".iter

  }

  protected def readSomeInternals(implicit indent: Indentation): Iterator[String] = {

    def args = commas(sf.fields.map(_.name))

    "return MessageParser::read_fields(".iter ++ indent {
      "input,".iter ++ args
    } ++ ");".iter

  }

  protected def printInternals(implicit indent: Indentation): Iterator[String] = {
    def printArgs = commas(sf.fields.map(f => List(quoted(f.name), f.name)).flatten)

    "MessagePrinting::print_fields(".iter ++ indent {
      "printer,".iter ++ printArgs
    } ++ ");".iter
  }

  final override def impl(implicit indent: Indentation): Iterator[String] = {

    def license = commented(LicenseHeader.lines)

    def args = commas(sf.fields.map(_.name))

    def sizeFunc : Iterator[String] = {
      "size_t %s::size() const".format(sf.name).iter ++ bracket {
        "return MessageFormatter::sum_sizes(".iter ++ indent {
          "%s,".format(prefixSize).iter ++
          commas(sf.fields.map(f => f.name))
        } ++  ");".iter
      }
    }

    def readSomeFunc(implicit indent: Indentation): Iterator[String] = {
      "ParseError %s::read(seq32_t& input)".format(sf.name).iter ++ bracket {
        readSomeInternals
      }
    }

    def readAllFunc(implicit indent: Indentation): Iterator[String] = {
      "ParseError %s::read_all(const seq32_t& input)".format(sf.name).iter ++ bracket {
        Iterator(
          "auto remainder = input;",
          "auto err = read(remainder);",
          "if(any(err)) return err;",
          "return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;"
        )
      }
    }

    def writeFunc(implicit indent: Indentation): Iterator[String] = {
      "FormatError %s::write(wseq32_t& output) const".format(sf.name).iter ++ bracket {
        writeInternals
      }
    }

    def printFunc = "void %s::print(const char* name, IMessagePrinter& printer) const".format(sf.name).iter ++ bracket {
      printInternals
    }

    def defaultConstructorImpl(implicit indent: Indentation): Iterator[String] = {

      val defaults = sf.fields.flatMap { f =>
        f.cpp.defaultValue.map(d => "%s(%s)".format(f.name, d))
      }

      if (defaults.isEmpty) {
        "%s::%s()".format(sf.name, sf.name).iter ++ bracketsOnly
      }
      else {

        def sig = "%s::%s() : ".format(sf.name, sf.name).iter

        sig ++ indent(commas(defaults)) ++ bracketsOnly
      }
    }

    def fullConstructorImpl(implicit indent: Indentation): Iterator[String] = {

      val constructedFields = sf.fields.filter(_.cpp.initializeInFullConstructor)

      if(constructedFields.isEmpty) return Iterator.empty

      def names: List[String] = constructedFields.map(f => "%s(%s)".format(f.name, f.name))

      fullConstructorSig(true) ++ indent {
        commas(names)
      } ++ bracketsOnly
    }

    def readWritePrint: Iterator[String] = {
      if (outputReadWritePrint) {
        readSomeFunc ++ space ++ readAllFunc ++ space ++ writeFunc ++ space ++ printFunc
      }
      else {
        Iterator.empty
      }
    }

    def funcs = {
        defaultConstructorImpl ++
        space ++
        fullConstructorImpl ++
        space ++
        sizeFunc ++
        space ++
        readWritePrint ++
        extraImplFunctions
    }

    def selfInclude = Includes.message(sf.name, public).line

    def includes = {
      Includes.lines(List(Includes.msgParser, Includes.msgFormatter, Includes.msgPrinting))
    }

    license ++ space ++ selfInclude ++ space ++ includes ++ space ++ namespace(cppNamespace)(funcs)

  }

  private def fullConstructorSig(impl: Boolean)(implicit indent: Indentation): Iterator[String] = {

    val fields = sf.fields.filter(_.cpp.initializeInFullConstructor)

    if(fields.isEmpty) return Iterator.empty

    val firstArgs: Iterator[String] = fields.dropRight(1).map(f => f.cpp.asArgument(f.name) + ",").toIterator

    val last = fields.last
    val lastArgs: Iterator[String] = Iterator(last.cpp.asArgument(last.name))

    def funcName = if (impl) {
      "%s::%s(".format(sf.name, sf.name).iter
    } else {
      if (fields.size == 1) ("explicit " + sf.name + "(").iter else (sf.name + "(").iter
    }

    def terminator = if (impl) ") :".iter else ");".iter


    funcName ++ indent {
      firstArgs ++ lastArgs
    } ++ terminator
  }
}
