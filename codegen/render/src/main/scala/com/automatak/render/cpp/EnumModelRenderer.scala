/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumModelRenderer extends ModelRenderer[EnumModel] {

  def render(enum: EnumModel)(implicit indent: Indentation) : Iterator[String] = {

    def definition(ev: EnumValue, last : Boolean): String  = {
      val format = "%s = %s".format(ev.name, enum.render(ev.value))
      if(last) format
      else format + ","
    }

    def header: Iterator[String] = {
      "enum class %s : %s".format(enum.name, enum.enumType.cppType).iter
    }

    def lines(definition: String, comment: Option[String]) : Iterator[String] = comment match {
      case Some(c) => Iterator("/// %s".format(c), definition)
      case None => definition.iter
    }

    def definitionLines : Iterator[String] = {

      val head : Iterator[String] = enum.allValues.dropRight(1).map {
        ev => lines(definition(ev, false), ev.comment)
      }.flatten.toIterator

      val last = enum.allValues.last
      def tail = lines(definition(last, true), last.comment)

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
