package com.automatak.render.ssp21.messages.generators

import com.automatak.render.cpp._
import com.automatak.render.{EnumModel, EnumValue}


case class Include(file: String) {
  def line : String = "#include " + file
}

object Includes {
  val cstdint = Include("<cstdint>")
  val rslice = Include(quoted("openpal/container/RSlice.h"))
  val seqRSlice = Include(quoted("ssp21/SeqRSlice.h"))
}

sealed trait FieldGenerator {
  def includes : Set[Include]
  def cppType : String
}

object U16FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.cstdint)
  override def cppType : String = "uint16_t"
}

object U32FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.cstdint)
  override def cppType : String = "uint32_t"
}

case class EnumFieldGenerator(enum: EnumModel) extends FieldGenerator {
  override def includes = Set(Include(quoted("ssp21/gen/%s.h").format(enum.name)))
  override def cppType : String = enum.name
}

case class FixedEnumFieldGenerator(enum: EnumModel, value: EnumValue) extends FieldGenerator {
  override def includes = Set(Include(quoted("ssp21/gen/%s.h").format(enum.name)))
  override def cppType : String = enum.name
}

object Seq8FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.rslice)
  override def cppType : String = "openpal::RSlice"
}

object Seq16FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.rslice)
  override def cppType : String = "openpal::RSlice"
}

object Seq8Seq16FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.seqRSlice)
  override def cppType : String = "SeqRSlice"
}


