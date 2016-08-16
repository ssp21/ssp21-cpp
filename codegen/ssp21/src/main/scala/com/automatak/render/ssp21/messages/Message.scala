package com.automatak.render.ssp21.messages

import com.automatak.render.EnumModel

sealed trait Field {
  def name : String
}

sealed case class U16(name: String) extends Field
sealed case class U32(name: String) extends Field

sealed case class Enum(model: EnumModel) extends Field {
  def name : String = model.underscoredName
}

sealed case class Seq8(name: String) extends Field
sealed case class Seq16(name: String) extends Field
sealed case class Seq8Seq16(name: String) extends Field

trait Message {
  def name : String
  def fields : List[Field]
}
