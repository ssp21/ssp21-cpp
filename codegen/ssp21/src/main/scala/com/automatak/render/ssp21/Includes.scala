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

  val rslice = openpal("container/RSlice")
  val wslice = openpal("container/WSlice")
  val uncopyable = openpal("util/Uncopyable")
  val bigEndian = openpal("serialization/BigEndian")

  val parseError = enum("ParseError")
  val formatError = enum("FormatError")
  val function = enum("Function")

  val errorCategory = Include(quoted("ssp21/ErrorCategory.h"), Ordering.ssp21)

  val imessage = crypto("IMessage")
  val enumField = crypto("EnumField")

  val seqTypes = crypto("SequenceTypes")
  val msgFormatter = crypto("MessageFormatter")
  val formatResult = crypto("FormatResult")
  val msgPrinter = crypto("IMessagePrinter")
  val msgPrinting = crypto("MessagePrinting")
  val flagsPrinting = crypto("FlagsPrinting")
  val msgParser = crypto("MessageParser")

  val messageField = List(parseError, formatError, msgPrinter, wslice, rslice)

  def crypto(className: String) = Include(quoted("ssp21/crypto/%s.h".format(className)), Ordering.crypto)
  def enum(className: String) = Include(quoted("ssp21/crypto/gen/%s.h".format(className)), Ordering.enum)
  def message(className: String) = Include(quoted("ssp21/crypto/gen/%s.h".format(className)), Ordering.msg)
  def openpal(classPath: String)= Include(quoted("openpal/%s.h".format(classPath)), Ordering.openpal)

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

