package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.WriteCppFiles
import com.automatak.render.ssp21.messages.generators._
import com.automatak.render.{EnumModel, EnumValue}

sealed trait Field {
  def name : String
  def cpp : FieldGenerator
  def minSizeBytes : Int
}

case class Bit(name: String, default: Boolean)

trait Bitfield extends  Field {
  def structName : String
  def files : WriteCppFiles = BitfieldStructGenerator(this)
  def bits: List[Bit]
}

object Bitfield {

  private case class Bits(name: String, structName: String, bits: List[Bit]) extends Bitfield {
    def cpp = BitfieldGenerator(this)
    def minSizeBytes : Int = 1
  }

  def apply(name: String, structName: String, bit1: Bit, bit2: Bit): Bitfield = {
    Bits(name, structName, List(bit1, bit2))
  }
}

sealed case class U16(name: String) extends Field {
  def cpp = U16FieldGenerator
  def minSizeBytes : Int = 2
}
sealed case class U32(name: String) extends Field {
  def cpp = U32FieldGenerator
  def minSizeBytes : Int = 4
}

sealed case class Enum(model: EnumModel) extends Field {
  def name : String = model.underscoredName
  def cpp = EnumFieldGenerator(model)
  def minSizeBytes : Int = 1
}

sealed case class Seq8(name: String) extends Field {
  def cpp = Seq8FieldGenerator
  def minSizeBytes : Int = 1
}

sealed case class Seq16(name: String) extends Field {
  def cpp = Seq16FieldGenerator
  def minSizeBytes : Int = 2
}

sealed case class Seq8Seq16(name: String) extends Field {
  def cpp = Seq8Seq16FieldGenerator
  def minSizeBytes : Int = 1
}

trait Message {
  def name : String
  def fields : List[Field]
  def function : EnumValue
  def minSizeBytes = fields.foldLeft(0)(_ + _.minSizeBytes) + 1
}
