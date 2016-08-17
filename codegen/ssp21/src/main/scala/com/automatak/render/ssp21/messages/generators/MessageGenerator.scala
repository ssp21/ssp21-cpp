package com.automatak.render.ssp21.messages.generators

import java.nio.file.Path

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.ssp21.Includes
import com.automatak.render.ssp21.messages.Message

object MessageGenerator {

  def apply(messages: List[Message], cppNamespace : String, incFormatString: String, incDirectory: Path, implDirectory: Path): Unit = {

    implicit val indent = CppIndentation()

    def headerPath(m: Message) = incDirectory.resolve(headerName(m))
    def implPath(m: Message) = implDirectory.resolve(implName(m))

    def headerName(m: Message) = m.name + ".h"
    def implName(m: Message) = m.name + ".cpp"

    def writeToFiles(message: Message): Unit = {

      def includes : Iterator[String] = {
        Includes.lines(
          List(
            Includes.rslice,
            Includes.wslice,
            Includes.uncopyable,
            Includes.parseError,
            Includes.formatError,
            Includes.formatResult
          ) ::: message.fields.flatMap(f => f.cpp.includes.toList)
        )
      }

      def defaultConstructorSig = "%s();".format(message.name).iter

      def fullConstructorSig(impl: Boolean) : Iterator[String] = {

        val fields = message.fields.filter(_.cpp.initializeInFullConstructor)

        val firstArgs : Iterator[String] = fields.dropRight(1).map(f => f.cpp.asArgument(f.name)+",").toIterator

        val last = fields.last
        val lastArgs : Iterator[String] = Iterator(last.cpp.asArgument(last.name))

        def funcName = if(impl) "%s::%s(".format(message.name, message.name).iter else (message.name + "(").iter

        def terminator = if(impl) ") :".iter else ");".iter

        funcName ++ indent {
          firstArgs ++ lastArgs
        } ++ terminator
      }


      def readSigHeader = "ParseError read(openpal::RSlice& input);".iter

      def writeSigHeader = "FormatError write(openpal::WSlice& output);".iter

      def writeMsgSig = "FormatResult write_msg(openpal::WSlice& output);".iter

      def minSizeBytes = "static const uint32_t min_size_bytes = %s;".format(message.minSizeBytes).iter

      def fieldDefintions : Iterator[String] = message.fields.map { f =>
        "%s %s;".format(f.cpp.cppType, f.name);
      }.toIterator

      def struct : Iterator[String] = {

        "struct %s : private openpal::Uncopyable".format(message.name).iter ++ bracketSemiColon {
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
        def lines = license ++ space ++ includeGuards(message.name)(includes ++ space ++ namespace(cppNamespace)(content))

        val path = headerPath(message)
        writeTo(path)(lines)
        println("Wrote: " + path)
      }

      def defaultConstructorImpl : Iterator[String] = {

        val defaults = message.fields.flatMap { f =>
          f.cpp.defaultValue.map(d => "%s(%s)".format(f.name, d))
        }

        if(defaults.isEmpty) {
          "%s::%s()".format(message.name, message.name).iter ++ bracketsOnly
        }
        else {

          def sig = "%s::%s() : ".format(message.name, message.name).iter

          sig ++ indent(commas(defaults)) ++ bracketsOnly
        }
      }

      def fullConstructorImpl : Iterator[String] = {

        def constructedFields = message.fields.filter(_.cpp.initializeInFullConstructor)
        def names : List[String] = constructedFields.map(f => "%s(%s)".format(f.name, f.name))

        fullConstructorSig(true) ++ indent {
          commas(names)
        } ++ bracketsOnly
      }

      def readFunc : Iterator[String] = {

        def args = message.fields.map(_.name)

        "ParseError %s::read(openpal::RSlice& input)".format(message.name).iter ++ bracket {
          "return MessageParser::read_message<Function::%s>(".format(message.function.name).iter ++ indent {
            "input,".iter ++
              commas(args)
          } ++ ");".iter
        }
      }

      def writeFunc : Iterator[String] = {

        "FormatError %s::write(openpal::WSlice& output)".format(message.name).iter ++ bracket {
          "return MessageFormatter::write_message<Function::%s>(".format(message.function.name).iter ++ indent {
            "output,".iter ++ commas(message.fields.map(_.name))
          } ++ ");".iter
        }

      }

      def writeMsgFunc : Iterator[String] = {

          "FormatResult %s::write_msg(openpal::WSlice& output)".format(message.name).iter ++ bracket {
            "auto write = [this](openpal::WSlice& output) { return this->write(output); };".iter ++
            "return FormatResult::write_any(write, output);".iter
          }
      }

      def writeImpl() {


        def license = commented(LicenseHeader())
        def funcs = defaultConstructorImpl ++ space ++ fullConstructorImpl ++ space ++ readFunc ++ space ++ writeFunc ++ space ++ writeMsgFunc

        def selfInclude = include(quoted(String.format(incFormatString, headerName(message))))

        def includes = {
          selfInclude ++
          space ++
          Includes.lines(List(Includes.msgParser, Includes.msgFormatter))
        }

        def lines = license ++ space ++ includes ++ space ++ namespace(cppNamespace)(
          funcs
        )

        val path = implPath(message)
        writeTo(path)(lines)
        println("Wrote: " + path)
      }

      writeHeader()
      writeImpl()
    }

    messages.foreach(writeToFiles)
  }
}
