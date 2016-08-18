/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumModelRenderer extends ModelRenderer[EnumModel] {

  def render(enum: EnumModel)(implicit indent: Indentation) : Iterator[String] = {

    def lastDefinition(ev: EnumValue): String  = {
      "%s = %s".format(ev.name, enum.render(ev.value))
    }

    def leadingDefinition(ev: EnumValue): String = lastDefinition(ev) + ","

    def header: Iterator[String] = {
      "enum class %s : %s".format(enum.name, enum.cpp.typ).iter
    }

    def lines(definition: String, comment: String) : Iterator[String] = {
      Iterator("/// %s".format(comment), definition)
    }

    def definitionLines : Iterator[String] = {

      val head : Iterator[String] = enum.allValues.dropRight(1).map {
        ev => lines(leadingDefinition(ev), ev.comment)
      }.flatten.toIterator

      val last = enum.allValues.last
      def tail = lines(lastDefinition(last), last.comment)

      head ++ tail
    }

    def summary = if(enum.comments.isEmpty) Iterator.empty else {
      "/**".iter ++ indent {
        enum.comments.toIterator
      } ++ "*/".iter
    }

    summary ++
    header ++ bracketSemiColon {
      definitionLines
    }

  }

}
