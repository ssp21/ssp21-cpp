package com.automatak.render.ssp21.messages.generators

import com.automatak.render.cpp._
import com.automatak.render.{EnumModel, EnumValue}


object Ordering {
  val system = 0
  val openpal = 1
  val ssp21 = 2
}

case class Include(file: String, order: Int) {
  def line : String = "#include " + file
}

object Includes {

  val cstdint = Include("<cstdint>", Ordering.system)
  val rslice = Include(quoted("openpal/container/RSlice.h"), Ordering.openpal)
  val wslice = Include(quoted("openpal/container/WSlice.h"), Ordering.openpal)
  val seqTypes = Include(quoted("ssp21/SequenceTypes.h"), Ordering.ssp21)
  val uncopyable = Include(quoted("openpal/util/Uncopyable.h"), Ordering.openpal)
  val parseError = Include(quoted("ssp21/gen/ParseError.h"), Ordering.ssp21)

  def lines(i : Seq[Include]) : Iterator[String] = {
    val sorted : List[Include] = i.toSet.toList.sortWith((lhs, rhs) => lhs.order > rhs.order)
    sorted.map(_.line).toIterator
  }
}

sealed trait FieldGenerator {
  def includes : Set[Include]
  def cppType : String
  def defaultValue: Option[String]
}

object U16FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.cstdint)
  override def cppType : String = "uint16_t"
  def defaultValue: Option[String] = Some("0")
}

object U32FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.cstdint)
  override def cppType : String = "uint32_t"
  def defaultValue: Option[String] = Some("0")
}

case class EnumFieldGenerator(enum: EnumModel) extends FieldGenerator {
  override def includes = Set(Include(quoted("ssp21/gen/%s.h").format(enum.name), Ordering.ssp21))
  override def cppType : String = enum.name
  def defaultValue: Option[String] = Some("%s::undefined".format(enum.name))
}

object Seq8FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.seqTypes)
  override def cppType : String = "Seq8"
  def defaultValue: Option[String] = None
}

object Seq16FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.seqTypes)
  override def cppType : String = "Seq16"
  def defaultValue: Option[String] = None
}

object Seq8Seq16FieldGenerator extends FieldGenerator {
  override def includes = Set(Includes.seqTypes)
  override def cppType : String = "Seq8Seq16"
  def defaultValue: Option[String] = None
}


