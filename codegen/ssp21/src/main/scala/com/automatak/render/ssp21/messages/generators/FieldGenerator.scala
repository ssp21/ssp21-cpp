package com.automatak.render.ssp21.messages.generators

import com.automatak.render.ssp21.messages.{Bitfield, StructField}
import com.automatak.render.ssp21.{Include, Includes, Ordering}
import com.automatak.render.{EnumModel, _}


sealed trait FieldGenerator {
  def includes: Set[Include]

  def cppType: String

  def defaultValue: Option[String]

  def asArgument(name: String): String

  def initializeInFullConstructor: Boolean = true
}

sealed trait PassByValue {
  self: FieldGenerator =>
  def asArgument(name: String): String = "%s %s".format(cppType, name)
}

sealed trait PassByConstRef {
  self: FieldGenerator =>
  def asArgument(name: String): String = "const %s& %s".format(cppType, name)
}

case class BitfieldGenerator(field: Bitfield) extends FieldGenerator with PassByConstRef {
  override def includes = Set(Includes.message(field.name))

  override def cppType: String = field.name

  def defaultValue: Option[String] = None
}

case class StructFieldGenerator(sf: StructField) extends FieldGenerator with PassByConstRef {
  override def includes = Set(Includes.message(sf.struct.name))

  override def cppType: String = sf.struct.name

  def defaultValue: Option[String] = None
}

object U16FieldGenerator extends FieldGenerator with PassByValue {
  override def includes = Set(Includes.cstdint)

  override def cppType: String = "uint16_t"

  def defaultValue: Option[String] = Some("0")
}

object U32FieldGenerator extends FieldGenerator with PassByValue {
  override def includes = Set(Includes.cstdint)

  override def cppType: String = "uint32_t"

  def defaultValue: Option[String] = Some("0")
}

case class EnumFieldGenerator(enum: EnumModel) extends FieldGenerator with PassByValue {
  override def includes = Set(Includes.enum(enum.name), Includes.enumField)

  override def cppType: String = "EnumField<%s>".format(enum.specName)

  def defaultValue: Option[String] = Some("%s::undefined".format(enum.name))
}

object Seq8FieldGenerator extends FieldGenerator with PassByConstRef {
  override def includes = Set(Includes.seqTypes)

  override def cppType: String = "Seq8"

  def defaultValue: Option[String] = None
}

object Seq16FieldGenerator extends FieldGenerator with PassByConstRef {
  override def includes = Set(Includes.seqTypes)

  override def cppType: String = "Seq16"

  def defaultValue: Option[String] = None
}

object Seq8Seq16FieldGenerator extends FieldGenerator with PassByConstRef {
  override def includes = Set(Includes.seqTypes)

  override def cppType: String = "Seq8Seq16"

  def defaultValue: Option[String] = None

  override def initializeInFullConstructor: Boolean = false
}


