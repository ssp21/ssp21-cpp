/**
 * License TBD
 */
package com.automatak

import java.nio.file.{Files, StandardOpenOption, Path}
import java.nio.charset.Charset

package object render {

    implicit class RichString(s : String) {
      def iter : Iterator[String] = Iterator(s)
    }

    // adds commas to all but the last item
    def mapAllButLast(items : Seq[String])(func: String => String) : Iterator[String] = {
      if(items.isEmpty) items.toIterator
      else (items.dropRight(1).map(func) ++ List(items.last)).toIterator
    }

    def commas(items : Seq[String]) : Iterator[String] = mapAllButLast(items)(_ + ",")

    def space: Iterator[String] = "".iter

    def commented(lines: Iterator[String]): Iterator[String] = {
      Iterator("//") ++ lines.map(l => "// " + l) ++ Iterator("//")
    }

    def bracketWithCap(indent: Indentation, cap: String)(inner: => Iterator[String]): Iterator[String] = {
      Iterator("{") ++
        indent(
          inner
        ) ++
        Iterator("}"+cap)
    }

    def bracket(inner: => Iterator[String])(implicit indent: Indentation): Iterator[String] = bracketWithCap(indent,"")(inner)

    def bracketsOnly : Iterator[String] = Iterator("{}")

    def bracketSemiColon(inner: => Iterator[String])(implicit indent: Indentation): Iterator[String] = bracketWithCap(indent,";")(inner)

    def bracketSemiColon(cap: String)(inner: => Iterator[String])(implicit indent: Indentation): Iterator[String] = bracketWithCap(indent," %s;".format(cap))(inner)

    def writeTo(path: Path)(lines: Iterator[String]): Unit = {

      if(!lines.isEmpty)
      {
        if(!Files.exists(path.getParent)) Files.createDirectory(path.getParent)
        if(!Files.exists(path)) Files.createFile(path)

        val writer = Files.newBufferedWriter( path, Charset.defaultCharset, StandardOpenOption.TRUNCATE_EXISTING)

        def writeLine(s: String) = {
          writer.write(s)
          writer.write(System.lineSeparator)
        }

        try { lines.foreach(writeLine) }
        finally { writer.close() }
      }



    }
}
