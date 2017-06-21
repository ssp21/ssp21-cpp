package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.messages.generators._
import com.automatak.render.{EnumModel, EnumValue}

sealed trait Field {
  def name: String

  def cpp: FieldGenerator

  def minSizeBytes: Int

  def fixedSize: Option[Int]
}

case class Bit(name: String, default: Boolean)

trait Bitfield {
  def name: String

  def bits: List[Bit]
}

case class BitfieldField(name: String, bf: Bitfield) extends Field {

  def cpp = BitfieldGenerator(bf)

  def minSizeBytes: Int = 1

  def fixedSize: Option[Int] = Some(1)
}

sealed case class U8(name: String) extends Field {
  def cpp = U8FieldGenerator

  def minSizeBytes: Int = 1

  def fixedSize: Option[Int] = Some(1)
}

sealed case class U16(name: String) extends Field {
  def cpp = U16FieldGenerator

  def minSizeBytes: Int = 2

  def fixedSize: Option[Int] = Some(2)
}

sealed case class U32(name: String) extends Field {
  def cpp = U32FieldGenerator

  def minSizeBytes: Int = 4

  def fixedSize: Option[Int] = Some(4)
}

sealed case class U64(name: String) extends Field {
  def cpp = U64FieldGenerator

  def minSizeBytes: Int = 8

  def fixedSize: Option[Int] = Some(8)
}

sealed case class Enum(model: EnumModel) extends Field {
  def name: String = model.underscoredName

  def cpp = EnumFieldGenerator(model)

  def minSizeBytes: Int = 1

  def fixedSize: Option[Int] = Some(1)
}

sealed case class SeqOfByte(name: String) extends Field {
  def cpp = SeqOfByteFieldGenerator

  def minSizeBytes: Int = 1

  def fixedSize: Option[Int] = None
}

sealed case class SeqOfStruct(name: String, struct: Struct, capacity: Int) extends Field {

  def cpp = SeqOfStructGenerator(struct, capacity)

  def minSizeBytes: Int = 1

  def fixedSize: Option[Int] = None
}

trait Struct {

  def name: String

  def fields: List[Field]


  def minSizeBytes: Int = fields.foldLeft(0)(_ + _.minSizeBytes)

  def fixedSize: Option[Int] = {
    if (!fields.forall(f => f.fixedSize.isDefined)) None
    else {
      Some(fields.flatMap(f => f.fixedSize).sum)
    }
  }
}

case class StructField(name: String, struct: Struct) extends Field {
  def cpp: FieldGenerator = StructFieldGenerator(this)

  def minSizeBytes: Int = struct.minSizeBytes

  def fixedSize: Option[Int] = struct.fixedSize
}

trait Message extends Struct {
  def function: EnumValue

  override def minSizeBytes = super.minSizeBytes + 1

  override def fixedSize: Option[Int] = super.fixedSize.map(_ + 1)
}
