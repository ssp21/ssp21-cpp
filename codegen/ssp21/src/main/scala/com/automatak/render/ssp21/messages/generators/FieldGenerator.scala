package com.automatak.render.ssp21.messages.generators

import com.automatak.render.cpp._
import com.automatak.render.{EnumModel, EnumValue}


case class Include(file: String) {
  def line : String = "#include " + file
}

object Includes {
  val cstdint = Include("<cstdint>")
  val rslice = Include(quoted("openpal/container/RSlice.h"))
}

sealed trait FieldGenerator {
  def includes : Set[Include]
}

object U16FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.cstdint)
}

object U32FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.cstdint)
}

case class EnumFieldGenerator(enum: EnumModel) extends FieldGenerator {
  override def includes = Set(Include(quoted("ssp21/gen/%s.h").format(enum.name)))
}

case class FixedEnumFieldGenerator(enum: EnumModel, value: EnumValue) extends FieldGenerator {
  override def includes = Set(Include(quoted("ssp21/gen/%s.h").format(enum.name)))
}

object Seq8FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.rslice)
}

object Seq16FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.rslice)
}

object Seq8Seq16FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.rslice)
}


