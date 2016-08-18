package com.automatak.render.ssp21.messages.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21.messages.Message
import com.automatak.render.ssp21.{Includes, WriteCppFiles}


case class MessageGenerator(msg: Message) extends WriteCppFiles {

  private def cppNamespace = "ssp21"
  private def headerName = msg.name + ".h"
  private def implName = msg.name + ".cpp"

  override def mainClassName: String = msg.name

  override def header(implicit indent: Indentation) : Iterator[String] = {

    def readSigHeader = "ParseError read_msg(openpal::RSlice& input);".iter
    def writeMsgSig = "FormatResult write_msg(openpal::WSlice& output);".iter
    def writeSigHeader = "FormatError write(openpal::WSlice& output);".iter
    def defaultConstructorSig = "%s();".format(msg.name).iter

    def minSizeBytes = "static const uint32_t min_size_bytes = %s;".format(msg.minSizeBytes).iter

    def struct(implicit indent: Indentation) : Iterator[String] = {

      def fieldDefintions : Iterator[String] = msg.fields.map { f =>
        "%s %s;".format(f.cpp.cppType, f.name);
      }.toIterator

      "struct %s : private openpal::Uncopyable".format(msg.name).iter ++ bracketSemiColon {
        defaultConstructorSig ++
          space ++
          fullConstructorSig(false) ++
          space ++
          readSigHeader ++
          writeMsgSig ++
          space ++
          minSizeBytes ++
          space ++
          fieldDefintions ++
          space ++
          "private: ".iter ++
          space ++
          writeSigHeader
      }
    }

    def includes : Iterator[String] = {
      Includes.lines(
        List(
          Includes.rslice,
          Includes.wslice,
          Includes.uncopyable,
          Includes.parseError,
          Includes.formatError,
          Includes.formatResult
        ) ::: msg.fields.flatMap(f => f.cpp.includes.toList)
      )
    }

    def license = commented(LicenseHeader())
    def content = struct

    license ++ space ++ includeGuards(msg.name)(includes ++ space ++ namespace(cppNamespace)(content))
  }

  override def impl(implicit indent: Indentation) : Iterator[String] = {

    def license = commented(LicenseHeader())

    def args = commas(msg.fields.map(_.name))

    def readFunc(implicit indent: Indentation) : Iterator[String] = {

      "ParseError %s::read_msg(openpal::RSlice& input)".format(msg.name).iter ++ bracket {
        "return MessageParser::read_message<Function::%s>(".format(msg.function.name).iter ++ indent {
          "input,".iter ++ args
        } ++ ");".iter
      }
    }

    def writeFunc(implicit indent: Indentation) : Iterator[String] = {

      "FormatError %s::write(openpal::WSlice& output)".format(msg.name).iter ++ bracket {
        "return MessageFormatter::write_message<Function::%s>(".format(msg.function.name).iter ++ indent {
          "output,".iter ++ args
        } ++ ");".iter
      }

    }

    def writeMsgFunc(implicit indent: Indentation) : Iterator[String] = {

      "FormatResult %s::write_msg(openpal::WSlice& output)".format(msg.name).iter ++ bracket {
        "auto write = [this](openpal::WSlice& output) { return this->write(output); };".iter ++
          "return FormatResult::write_any(write, output);".iter
      }
    }

    def defaultConstructorImpl(implicit indent: Indentation) : Iterator[String] = {

      val defaults = msg.fields.flatMap { f =>
        f.cpp.defaultValue.map(d => "%s(%s)".format(f.name, d))
      }

      if(defaults.isEmpty) {
        "%s::%s()".format(msg.name, msg.name).iter ++ bracketsOnly
      }
      else {

        def sig = "%s::%s() : ".format(msg.name, msg.name).iter

        sig ++ indent(commas(defaults)) ++ bracketsOnly
      }
    }

    def fullConstructorImpl(implicit indent: Indentation) : Iterator[String] = {

      def constructedFields = msg.fields.filter(_.cpp.initializeInFullConstructor)
      def names : List[String] = constructedFields.map(f => "%s(%s)".format(f.name, f.name))

      fullConstructorSig(true) ++ indent {
        commas(names)
      } ++ bracketsOnly
    }

    def funcs = defaultConstructorImpl ++ space ++ fullConstructorImpl ++ space ++ readFunc ++ space ++ writeFunc ++ space ++ writeMsgFunc

    def selfInclude = include(quoted("ssp21/msg/%s".format(headerName)))

    def includes = {
      selfInclude ++
      space ++
      Includes.lines(List(Includes.msgParser, Includes.msgFormatter))
    }

    license ++ space ++ includes ++ space ++ namespace(cppNamespace)(funcs)

  }

  private def fullConstructorSig(impl: Boolean)(implicit indent: Indentation) : Iterator[String] = {

    val fields = msg.fields.filter(_.cpp.initializeInFullConstructor)

    val firstArgs : Iterator[String] = fields.dropRight(1).map(f => f.cpp.asArgument(f.name)+",").toIterator

    val last = fields.last
    val lastArgs : Iterator[String] = Iterator(last.cpp.asArgument(last.name))

    def funcName = if(impl) "%s::%s(".format(msg.name, msg.name).iter else (msg.name + "(").iter

    def terminator = if(impl) ") :".iter else ");".iter

    funcName ++ indent {
      firstArgs ++ lastArgs
    } ++ terminator
  }
}
