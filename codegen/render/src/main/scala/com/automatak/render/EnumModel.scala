/**
  * License TBD
  */
package com.automatak.render


object EnumValue {

  def undefined(value: Int) = EnumValue("undefined", value, "value not defined")

  def list(names: String*) : List[EnumValue] = {
    names.zipWithIndex.map(x => EnumValue(x._1, x._2,"")).toList
  }
}

case class EnumValue(name: String, value: Int, comment: String)

sealed trait IntRender {
  def apply(i: Int): String
}

case object Hex extends IntRender {
  def apply(i: Int): String = "0x" + Integer.toHexString(i).toUpperCase
}

case object Base10 extends IntRender {
  def apply(i: Int): String = i.toString
}

case class ErrorCategory(className: String, stringName: String)

trait EnumModel {

  // required definitions

  def name: String

  def underscoredName: String

  def comments: List[String]

  def nonDefaultValues: List[EnumValue]

  def defaultValue: Option[EnumValue]

  // optional overrides
  def enumType: EnumType.Type = EnumType.UInt8

  def boolCastValue: Option[EnumValue] = None

  def render: IntRender = Hex

  def errorCategory: Option[ErrorCategory] = None

  def specName = name + "Spec"

  def allValues: List[EnumValue] = {
    val list = defaultValue match {
      case Some(d) => nonDefaultValues ::: List(d)
      case None => nonDefaultValues
    }
    list.sortBy(ev => ev.value)
  }

  def default: EnumValue = defaultValue match {
    case Some(x) => x
    case None => throw new Exception(name + " does not have a default value")
  }

  def qualified(ev: EnumValue): String = List(name, "::", ev.name).mkString
}
