package com.automatak.render.ssp21.messages.generators

import com.automatak.render._
import com.automatak.render.ssp21.messages.Bitfield

object BitfieldStruct {

  def definition(field: Bitfield)(implicit indent: Indentation) : Iterator[String] = {

    def members : Iterator[String] = field.bits.map(b => "bool %s = %b;".format(b.name, b.default)).toIterator
    def defaultConstructor = "%s(){}".format(field.structName).iter


    "struct %s".format(field.structName).iter ++ bracketSemiColon {
      defaultConstructor ++
      space ++
      members
    }
  }

}
