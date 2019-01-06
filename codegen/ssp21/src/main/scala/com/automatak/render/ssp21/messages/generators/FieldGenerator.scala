/**
 * License TBD
 */
package com.automatak.render.ssp21.messages.generators

import com.automatak.render.EnumModel
import com.automatak.render.ssp21.messages.{Bitfield, Struct, StructField}
import com.automatak.render.ssp21.{Include, Includes}


sealed trait FieldGenerator {
  def includes: Set[Include]

  def cppType: String

  def paramType: String = cppType

  def defaultValue: Option[String]

  def asArgument(name: String): String

  def initializeInFullConstructor: Boolean = true
}

sealed trait PassByValue {
  self: FieldGenerator =>
  def asArgument(name: String): String = "%s %s".format(paramType, name)
}

sealed trait PassByConstRef {
  self: FieldGenerator =>
  def asArgument(name: String): String = "const %s& %s".format(paramType, name)
}

case class BitfieldGenerator(field: Bitfield) extends FieldGenerator with PassByConstRef {
  override def includes = Set(Includes.message(field.name, false))

  override def cppType: String = field.name

  def defaultValue: Option[String] = None
}

case class StructFieldGenerator(sf: StructField) extends FieldGenerator with PassByConstRef {
  override def includes = Set(Includes.message(sf.struct.name, sf.struct.public))

  override def cppType: String = sf.struct.name

  def defaultValue: Option[String] = None
}

object U8FieldGenerator extends FieldGenerator with PassByValue {
  override def includes = Set(Includes.cstdint, Includes.integerField, Includes.bigEndian)

  override def cppType: String = "IntegerField<openpal::UInt8>"

  override def paramType: String = "uint8_t"

  def defaultValue: Option[String] = None
}

object U16FieldGenerator extends FieldGenerator with PassByValue {
  override def includes = Set(Includes.cstdint, Includes.integerField, Includes.bigEndian)

  override def cppType: String = "IntegerField<openpal::UInt16>"

  override def paramType: String = "uint16_t"

  def defaultValue: Option[String] = None
}

object U32FieldGenerator extends FieldGenerator with PassByValue {
  override def includes = Set(Includes.cstdint, Includes.integerField, Includes.bigEndian)

  override def cppType: String = "IntegerField<openpal::UInt32>"

  override def paramType: String = "uint32_t"

  def defaultValue: Option[String] = None
}

object U64FieldGenerator extends FieldGenerator with PassByValue {
  override def includes = Set(Includes.cstdint, Includes.integerField, Includes.bigEndian)

  override def cppType: String = "IntegerField<openpal::UInt64>"

  override def paramType: String = "uint64_t"

  def defaultValue: Option[String] = None
}

case class EnumFieldGenerator(enum: EnumModel) extends FieldGenerator with PassByValue {

  override def includes = Set(Includes.enum(enum.name, true), Includes.enumField)

  override def cppType: String = "EnumField<%s>".format(enum.specName)

  override def paramType: String = enum.name

  def defaultValue: Option[String] = None
}

object SeqOfByteFieldGenerator extends FieldGenerator with PassByConstRef {
  override def includes = Set(Includes.seqField)

  override def paramType: String = "seq32_t"

  override def cppType: String = "SeqByteField"

  def defaultValue: Option[String] = None
}

case class SeqOfStructGenerator(struct: Struct, capacity: Int) extends FieldGenerator with PassByConstRef {

  override def includes = Set(Includes.seqStructField, Includes.message(struct.name, struct.public))

  override def cppType: String = "SeqStructField<%s, %d>".format(struct.name, capacity)

  def defaultValue: Option[String] = None

  override def initializeInFullConstructor: Boolean = false
}




