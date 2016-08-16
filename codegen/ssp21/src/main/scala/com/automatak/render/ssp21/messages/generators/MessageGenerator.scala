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
        Iterator(Include(quoted("openpal/util/Uncopyable.h")).line) ++ message.fields.flatMap(f => f.cpp.includes).toSet.map((inc : Include) => inc.line).toIterator
      }

      def struct : Iterator[String] = {
        Iterator("struct " + message.name + " : openpal::Uncopyable") ++ bracketSemiColon {
          message.fields.map { f =>
            "%s %s;".format(f.cpp.cppType, f.name);
          }.toIterator
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

      def writeImpl() {
        def license = commented(LicenseHeader())
        //def funcs = renders.map(r => r.impl.render(cfg.model)).flatten.toIterator
        def inc = quoted(String.format(incFormatString, headerName(message)))
        def lines = license ++ space ++ Iterator(include(inc)) ++ space ++ namespace(cppNamespace)(Iterator.empty)

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
