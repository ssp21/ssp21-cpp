package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.messages.generators._
import com.automatak.render.{EnumModel, EnumValue}

sealed trait Field {
  def name : String
  def cpp : FieldGenerator
}

sealed case class U16(name: String) extends Field {
  def cpp = U16FieldGenerator
}
sealed case class U32(name: String) extends Field {
  def cpp = U32FieldGenerator
}

sealed case class Enum(model: EnumModel) extends Field {
  def name : String = model.underscoredName
  def cpp = EnumFieldGenerator(model)
}

sealed case class FixedEnum(model: EnumModel, value: EnumValue) extends Field {
  def name : String = model.underscoredName
  def cpp = FixedEnumFieldGenerator(model, value)
}

sealed case class Seq8(name: String) extends Field {
  def cpp = Seq8FieldGenerator
}

sealed case class Seq16(name: String) extends Field {
  def cpp = Seq16FieldGenerator
}

sealed case class Seq8Seq16(name: String) extends Field {
  def cpp = Seq8Seq16FieldGenerator
}

trait Message {
  def name : String
  def fields : List[Field]
}
