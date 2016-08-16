package com.automatak.render.ssp21.messages.generators

import java.nio.file.Path

import com.automatak.render._
import com.automatak.render.cpp._
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
          Includes.uncopyable :: message.fields.flatMap(f => f.cpp.includes.toList)
        )
      }

      def defaultConstructorSig : Iterator[String] = Iterator(message.name + "();")

      def fieldDefintions : Iterator[String] = message.fields.map { f =>
        "%s %s;".format(f.cpp.cppType, f.name);
      }.toIterator

      def struct : Iterator[String] = {
        Iterator("struct " + message.name + " : openpal::Uncopyable") ++ bracketSemiColon {
          defaultConstructorSig ++
          space ++
          fieldDefintions
        }
      }

      def writeHeader() {
        def license = commented(LicenseHeader())
        //def enum = EnumModelRenderer.render(cfg.model)
        //def signatures = renders.map(c => c.header.render(cfg.model)).flatten.toIterator
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
          val sig = "%s::%s() : ".format(message.name, message.name)
          def initFirst(s: (String, String)) = "%s(%s),".format(s._1, s._2)
          def initLast(s: (String, String)) = "%s(%s)".format(s._1, s._2)
          val leading = defaults.dropRight(1).map(initFirst).toIterator
          val last = Iterator(initLast(defaults.last))

          Iterator(sig) ++ indent {
            leading ++ last
          } ++ bracket(Iterator.empty)
        }
      }

      def writeImpl() {
        def license = commented(LicenseHeader())
        def funcs = defaultConstructorImpl
        def inc = quoted(String.format(incFormatString, headerName(message)))
        def lines = license ++ space ++ Iterator(include(inc)) ++ space ++ namespace(cppNamespace)(funcs)

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
