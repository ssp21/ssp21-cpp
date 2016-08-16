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

      def writeHeader() {
        def license = commented(LicenseHeader())
        //def enum = EnumModelRenderer.render(cfg.model)
        //def signatures = renders.map(c => c.header.render(cfg.model)).flatten.toIterator
        def lines = license ++ space ++ includeGuards(message.name)(cstdint ++ space ++ namespace(cppNamespace)(space))
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
