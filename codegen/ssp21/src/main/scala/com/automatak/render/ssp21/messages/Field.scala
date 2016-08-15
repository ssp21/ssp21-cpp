package com.automatak.render.ssp21.messages

import com.automatak.render.EnumModel

/// Core abstraction used for fields in a message
sealed trait Field {
  def name : String
}

sealed case class U16(name: String) extends Field
sealed case class U32(name: String) extends Field

sealed case class Enum(model: EnumModel) extends Field {
  def name : String = model.underscoredName
}

sealed case class Seq8ofU8(name: String) extends Field
sealed case class Seq16ofU8(name: String) extends Field
sealed case class Seq8ofSeq16ofU8(name: String) extends Field