package com.automatak.render.ssp21.messages.generators

import java.nio.file.Path

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21.Includes
import com.automatak.render.ssp21.messages.Message


case class MessageGenerator(msg: Message) {

  def cppNamespace = "ssp21"

  def write(incFormatString: String, incDirectory: Path, implDirectory: Path): Unit = {

    implicit val indent = CppIndentation()

    def headerPath = incDirectory.resolve(headerName)
    def implPath = implDirectory.resolve(implName)

    def headerName = msg.name + ".h"
    def implName = msg.name + ".cpp"

    def writeToFiles() : Unit = {

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

      def defaultConstructorSig = "%s();".format(msg.name).iter

      def fullConstructorSig(impl: Boolean) : Iterator[String] = {

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


      def readSigHeader = "ParseError read(openpal::RSlice& input);".iter

      def writeSigHeader = "FormatError write(openpal::WSlice& output);".iter

      def writeMsgSig = "FormatResult write_msg(openpal::WSlice& output);".iter

      def minSizeBytes = "static const uint32_t min_size_bytes = %s;".format(msg.minSizeBytes).iter

      def fieldDefintions : Iterator[String] = msg.fields.map { f =>
        "%s %s;".format(f.cpp.cppType, f.name);
      }.toIterator

      def struct : Iterator[String] = {

        "struct %s : private openpal::Uncopyable".format(msg.name).iter ++ bracketSemiColon {
          defaultConstructorSig ++
          space ++
          fullConstructorSig(false) ++
          space ++
          readSigHeader ++
          writeSigHeader ++
          writeMsgSig ++
          space ++
          minSizeBytes ++
          space ++
          fieldDefintions
        }
      }

      def writeHeader() {
        def license = commented(LicenseHeader())
        def content = struct
        def lines = license ++ space ++ includeGuards(msg.name)(includes ++ space ++ namespace(cppNamespace)(content))

        writeTo(headerPath)(lines)
        println("Wrote: " + headerPath)
      }

      def defaultConstructorImpl : Iterator[String] = {

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

      def fullConstructorImpl : Iterator[String] = {

        def constructedFields = msg.fields.filter(_.cpp.initializeInFullConstructor)
        def names : List[String] = constructedFields.map(f => "%s(%s)".format(f.name, f.name))

        fullConstructorSig(true) ++ indent {
          commas(names)
        } ++ bracketsOnly
      }

      def readFunc : Iterator[String] = {

        def args = msg.fields.map(_.name)

        "ParseError %s::read(openpal::RSlice& input)".format(msg.name).iter ++ bracket {
          "return MessageParser::read_message<Function::%s>(".format(msg.function.name).iter ++ indent {
            "input,".iter ++
              commas(args)
          } ++ ");".iter
        }
      }

      def writeFunc : Iterator[String] = {

        "FormatError %s::write(openpal::WSlice& output)".format(msg.name).iter ++ bracket {
          "return MessageFormatter::write_message<Function::%s>(".format(msg.function.name).iter ++ indent {
            "output,".iter ++ commas(msg.fields.map(_.name))
          } ++ ");".iter
        }

      }

      def writeMsgFunc : Iterator[String] = {

          "FormatResult %s::write_msg(openpal::WSlice& output)".format(msg.name).iter ++ bracket {
            "auto write = [this](openpal::WSlice& output) { return this->write(output); };".iter ++
            "return FormatResult::write_any(write, output);".iter
          }
      }

      def writeImpl() {


        def license = commented(LicenseHeader())
        def funcs = defaultConstructorImpl ++ space ++ fullConstructorImpl ++ space ++ readFunc ++ space ++ writeFunc ++ space ++ writeMsgFunc

        def selfInclude = include(quoted(String.format(incFormatString, headerName)))

        def includes = {
          selfInclude ++
          space ++
          Includes.lines(List(Includes.msgParser, Includes.msgFormatter))
        }

        def lines = license ++ space ++ includes ++ space ++ namespace(cppNamespace)(
          funcs
        )

        writeTo(implPath)(lines)
        println("Wrote: " + implPath)
      }

      writeHeader()
      writeImpl()
    }

    writeToFiles()
  }
}
