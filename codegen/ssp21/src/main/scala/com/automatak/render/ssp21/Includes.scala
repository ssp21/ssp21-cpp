package com.automatak.render.ssp21

import com.automatak.render._
import com.automatak.render.cpp._

object Ordering {
  val system = 0
  val openpal = 1
  val enum = 2
  val msg = 3
  val ssp21 = 4
  val crypto = 5
}

case class Include(file: String, order: Int) {
  def line: Iterator[String] = include(file)
}

object Includes {

  val cstdint = Include("<cstdint>", Ordering.system)

  val uncopyable = openpal("util/Uncopyable")
  val bigEndian = openpal("serialization/BigEndian")

  val parseError = enum("ParseError")
  val formatError = enum("FormatError")
  val function = enum("Function")

  val errorCategory = ssp21("util/ErrorCategory")
  val seqTypes = ssp21("util/SequenceTypes")

  val imessage = crypto("IMessage")
  val enumField = crypto("EnumField")
  val integerField = crypto("IntegerField")

  val seqField = crypto("SeqByteField")
  val seqStructField = crypto("SeqStructField")
  val msgFormatter = crypto("MessageFormatter")
  val formatResult = crypto("FormatResult")
  val msgPrinter = crypto("IMessagePrinter")
  val msgPrinting = crypto("MessagePrinting")
  val flagsPrinting = crypto("FlagsPrinting")
  val msgParser = crypto("MessageParser")

  val messageField = List(parseError, formatError, msgPrinter, seqTypes)

  def ssp21(className: String) = Include(quoted("ssp21/%s.h".format(className)), Ordering.ssp21)

  def crypto(className: String) = Include(quoted("ssp21/crypto/%s.h".format(className)), Ordering.crypto)

  def enum(className: String, path: String): Include = Include(quoted("%s%s.h".format(path, className)), Ordering.enum)

  def enum(className: String): Include = enum(className, "ssp21/crypto/gen/")

  def message(className: String) = Include(quoted("ssp21/crypto/gen/%s.h".format(className)), Ordering.msg)

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

