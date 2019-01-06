/**
 * License TBD
 */
package com.automatak.render.ssp21

import com.automatak.render._
import com.automatak.render.cpp._

object Ordering {
  val system = 0
  val openpal = 1
  val public_ssp21 = 2
  val private_ssp21 = 3
}

case class Include(file: String, order: Int) {
  def line: Iterator[String] = include(file)
}

object Includes {

  val cstdint = Include("<cstdint>", Ordering.system)

  val uncopyable = openpal("util/Uncopyable")
  val bigEndian = openpal("serialization/BigEndian")

  val parseError = enum("ParseError", false)
  val formatError = enum("FormatError", false)
  val function = enum("Function", false)

  val errorCategory = ssp21("util/ErrorCategory", true)
  val seqTypes = ssp21("util/SequenceTypes", true)

  val imessage = crypto("IMessage", false)
  val enumField = crypto("EnumField", false)
  val integerField = crypto("IntegerField", false)

  val seqField = crypto("SeqByteField", false)
  val seqStructField = crypto("SeqStructField", false)
  val msgFormatter = crypto("MessageFormatter", false)
  val formatResult = crypto("FormatResult", false)
  val msgPrinter = crypto("IMessagePrinter", false)
  val msgPrinting = crypto("MessagePrinting", false)
  val flagsPrinting = crypto("FlagsPrinting", false)
  val msgParser = crypto("MessageParser", false)

  val messageField = List(parseError, formatError, msgPrinter, seqTypes)

  def enum(className: String, public: Boolean) = crypto("gen/%s".format(className), public)

  def message(className: String, public: Boolean) = crypto("gen/%s".format(className), public)

  def crypto(className: String, public: Boolean) = ssp21("crypto/%s".format(className), public)

  def ssp21(className: String, public: Boolean): Include = {
    public match {
      case true => Include(quoted("ssp21/%s.h".format(className)), Ordering.public_ssp21)
      case false => Include(quoted("%s.h".format(className)), Ordering.private_ssp21)
    }
  }

  def openpal(classPath: String) = Include(quoted("openpal/%s.h".format(classPath)), Ordering.openpal)

  def lines(lines: Seq[Include]): Iterator[String] = {

    def sort(lhs: Include, rhs: Include): Boolean = {
      if (lhs.order == rhs.order) {
        lhs.file.length < rhs.file.length
      }
      else {
        lhs.order < rhs.order
      }
    }

    lines.toSet.toList.sortWith(sort).map(_.line).flatten.toIterator
  }
}

