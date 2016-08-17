/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumModelRenderer extends ModelRenderer[EnumModel] {

  def render(enum: EnumModel)(implicit indent: Indentation) : Iterator[String] = {

    def pair(ir: IntRender)(ev: EnumValue): String = "%s = %s".format(ev.name, ir(ev.value))

    def header: Iterator[String] = {
      "enum class %s : %s".format(enum.name, enum.enumType.cppType).iter
    }

    def comments: Iterator[Option[Iterator[String]]] = {
      enum.allValues.map(ev => ev.comment.map(c => "/// %s".format(c).iter)).iterator
    }

    def definitions : Iterator[String] = commaDelimited(enum.allValues.map(pair(enum.render)).iterator)

    def summary = if(enum.comments.isEmpty) Iterator.empty else {
      "/**".iter ++ indent {
        enum.comments.toIterator
      } ++ "*/".iter
    }

    summary ++
    header ++ bracketSemiColon {
      merge(comments, definitions)
    }

  }

}
