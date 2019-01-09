/**
 * License TBD
 */
package com.automatak.render.ssp21

import com.automatak.render._
import com.automatak.render.cpp._

object Ordering {
  val system = 0
  val ser4cpp = 1
  val public_ssp21 = 2
  val private_ssp21 = 3
}

case class Include(file: String, order: Int) {
  def line: Iterator[String] = include(file)
}

object Includes {

  val cstdint = Include("<cstdint>", Ordering.system)

  val uncopyable = ser4cpp("util/Uncopyable")
  val bigEndian = ser4cpp("serialization/BigEndian")

  val parseError = enum("ParseError", true)
  val formatError = enum("FormatError", true)
  val function = enum("Function", false)

  val errorCategory = ssp21("util/ErrorCategory", true)
  val seqTypes = ssp21("util/SequenceTypes", true)

  val msgPrinter = crypto("IMessagePrinter", true)
  val enumField = crypto("EnumField", true)
  val integerField = crypto("IntegerField", true)
  val seqField = crypto("SeqByteField", true)
  val seqStructField = crypto("SeqStructField", true)

  val imessage = crypto("IMessage", false)
  val msgFormatter = crypto("MessageFormatter", false)
  val formatResult = crypto("FormatResult", false)
  val msgPrinting = crypto("MessagePrinting", false)
  val flagsPrinting = crypto("FlagsPrinting", false)
  val msgParser = crypto("MessageParser", false)

  val messageField = List(parseError, formatError, msgPrinter, seqTypes)

  def enum(className: String, public: Boolean) = crypto("gen/%s".format(className), public)

  def message(className: String, public: Boolean) = crypto("gen/%s".format(className), public)

  def crypto(className: String, public: Boolean) = ssp21("crypto/%s".format(className), public)

  def test(className: String) = ssp21("gen/%s".format(className), false)

  def ssp21(className: String, public: Boolean): Include = {
    public match {
      case true => Include(quoted("ssp21/%s.h".format(className)), Ordering.public_ssp21)
      case false => Include(quoted("%s.h".format(className)), Ordering.private_ssp21)
    }
  }

  def ser4cpp(classPath: String) = Include(quoted("ser4cpp/%s.h".format(classPath)), Ordering.ser4cpp)

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

