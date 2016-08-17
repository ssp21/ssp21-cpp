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

      def defaultConstructorSig : Iterator[String] = Iterator(message.name + "();")

      def fullConstructorSig(impl: Boolean) : Iterator[String] = {

        val fields = message.fields.filter(_.cpp.initializeInFullConstructor)

        val firstArgs : Iterator[String] = fields.dropRight(1).map(f => f.cpp.asArgument(f.name)+",").toIterator

        val last = fields.last
        val lastArgs : Iterator[String] = Iterator(last.cpp.asArgument(last.name))

        def funcName = if(impl) Iterator("%s::%s(".format(message.name, message.name)) else Iterator(message.name + "(")

        def terminator : Iterator[String] = if(impl) Iterator(") :") else Iterator(");")

        funcName ++ indent {
          firstArgs ++ lastArgs
        } ++ terminator
      }


      def readSigHeader : Iterator[String] = Iterator("ParseError read(openpal::RSlice& input);");

      def writeSigHeader : Iterator[String] = Iterator("FormatError write(openpal::WSlice& output);");

      def writeMsgSig : Iterator[String] = Iterator("FormatResult write_msg(openpal::WSlice& output);");

      def minSizeBytes : Iterator[String] = Iterator("static const uint32_t min_size_bytes = %s;".format(message.minSizeBytes))

      def fieldDefintions : Iterator[String] = message.fields.map { f =>
        "%s %s;".format(f.cpp.cppType, f.name);
      }.toIterator

      def struct : Iterator[String] = {
        Iterator("struct " + message.name + " : openpal::Uncopyable") ++ bracketSemiColon {
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

        val defaults : List[(String,String)] = message.fields.flatMap(f => f.cpp.defaultValue.map((f.name, _)))

        if(defaults.isEmpty) {
          Iterator("%s::%s()".format(message.name, message.name)) ++ bracket(Iterator.empty)
        }
        else {
          def sig = "%s::%s() : ".format(message.name, message.name)
          def initFirst(s: (String, String)) = "%s(%s),".format(s._1, s._2)
          def initLast(s: (String, String)) = "%s(%s)".format(s._1, s._2)
          def leading = defaults.dropRight(1).map(initFirst).toIterator
          def last = Iterator(initLast(defaults.last))

          Iterator(sig) ++ indent {
            leading ++ last
          } ++ bracket(Iterator.empty)
        }
      }

      def fullConstructorImpl : Iterator[String] = {

        val names = message.fields.filter(_.cpp.initializeInFullConstructor).map(_.name)

        def initFirst(name: String) = "%s(%s),".format(name, name)
        def initLast(name: String) = "%s(%s)".format(name, name)

        def leadingArgs = names.dropRight(1).map(initFirst).toIterator
        def lastArg = Iterator(initLast(names.last))


        fullConstructorSig(true) ++ indent {
          leadingArgs ++ lastArg
        } ++ Iterator("{}")
      }

      def readFunc : Iterator[String] = {

        def first = message.fields.dropRight(1).map(f => f.name + ",").toIterator
        def last = Iterator(message.fields.last.name)
        def args = first ++ last

        Iterator("ParseError %s::read(openpal::RSlice& input)".format(message.name)) ++ bracket {
          Iterator("return MessageParser::read_message<Function::%s>(".format(message.function.name)) ++ indent {
            Iterator("input,") ++
            args
          } ++ Iterator(");")
        }
      }

      def writeFunc : Iterator[String] = {

        def first = message.fields.dropRight(1).map(f => f.name + ",").toIterator
        def last = Iterator(message.fields.last.name)
        def args = first ++ last

        Iterator("FormatError %s::write(openpal::WSlice& output)".format(message.name)) ++ bracket {
          Iterator("return MessageFormatter::write_message<Function::%s>(".format(message.function.name)) ++ indent {
            Iterator("output,") ++
              args
          } ++ Iterator(");")
        }
      }

      def writeMsgFunc : Iterator[String] = {


          Iterator("FormatResult %s::write_msg(openpal::WSlice& output)".format(message.name)) ++ bracket {
            Iterator("auto write = [this](openpal::WSlice& output) { return this->write(output); };") ++
            Iterator("return FormatResult::write_any(write, output);")
          }


      }

      def writeImpl() {


        def license = commented(LicenseHeader())
        def funcs = defaultConstructorImpl ++ space ++ fullConstructorImpl ++ space ++ readFunc ++ space ++ writeFunc ++ space ++ writeMsgFunc

        def selfInclude = include(quoted(String.format(incFormatString, headerName(message))))

        def includes = {
          Iterator(selfInclude) ++
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