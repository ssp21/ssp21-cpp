/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumModelRenderer extends ModelRenderer[EnumModel] {

  def render(enum: EnumModel)(implicit indent: Indentation) : Iterator[String] = {

    def pair(ir: IntRender)(ev: EnumValue): String = List(ev.name, "=", ir(ev.value)).spaced

    def header: Iterator[String] = Iterator(List("enum","class", enum.name, ":", enum.enumType.cppType).spaced)

    def comments: Iterator[Option[Iterator[String]]] = enum.allValues.map(ev => ev.comment.map(c => Iterator("/// " + c))).iterator

    def definitions : Iterator[String] = commaDelimited(enum.allValues.map(pair(enum.render)).iterator)

    def summary = if(enum.comments.isEmpty) Iterator.empty else {
      Iterator("/**") ++ indent(enum.comments.toIterator) ++ Iterator("*/")
    }

    summary ++
    header ++ bracketSemiColon {
      merge(comments, definitions)
    }

  }

}
