/**
 * License TBD
 */
package com.automatak.render


object EnumValue {
  def undefined(value: Int) = EnumValue("undefined", value, "value not defined")
}

case class EnumValue(name: String, value: Int, comment: String, strName: Option[String] = None) {

  def displayName : String = strName.getOrElse(name)

}

object EnumModel {

  sealed trait CppType {
    def sizeInBytes: Int
    def typ : String
  }

  case object UInt8 extends CppType {
    def sizeInBytes = 1
    def typ = "uint8_t"
  }

  case object UInt16 extends CppType {
    def sizeInBytes = 2
    def typ = "uint16_t"
  }

  case object UInt32 extends CppType {
    def sizeInBytes = 4
    def typ = "uint32_t"
  }
}

sealed trait IntRender { def apply(i: Int): String }
case object Hex extends IntRender { def apply(i: Int): String = "0x"+Integer.toHexString(i).toUpperCase }
case object Base10 extends IntRender { def apply(i: Int): String = i.toString }

case class EnumModel(
                      name: String,
                      underscoredName: String,
                      comments: List[String],
                      cpp: EnumModel.CppType,
                      nonDefaultValues: List[EnumValue],
                      defaultValue: Option[EnumValue],
                      boolCastValue: Option[EnumValue] = None,
                      render: IntRender = Base10) {

  def specName = name + "Spec"

  def allValues: List[EnumValue] = defaultValue match {
    case Some(d) => nonDefaultValues ::: List(d)
    case None => nonDefaultValues
  }

  def default : EnumValue = defaultValue match {
    case Some(x) => x
    case None => throw new Exception(name + " does not have a default value")
  }

  def qualified(ev: EnumValue): String = List(name,"::",ev.name).mkString
}
