package com.automatak.render.ssp21

import com.automatak.render._
import com.automatak.render.cpp._

object Ordering {
  val system = 0
  val openpal = 1
  val enum = 2
  val msg = 3
  val crypto = 4
}

case class Include(file: String, order: Int) {
  def line: Iterator[String] = include(file)
}

object Includes {

  val cstdint = Include("<cstdint>", Ordering.system)

  val rslice = Include(quoted("openpal/container/RSlice.h"), Ordering.openpal)
  val wslice = Include(quoted("openpal/container/WSlice.h"), Ordering.openpal)
  val uncopyable = Include(quoted("openpal/util/Uncopyable.h"), Ordering.openpal)
  val bigEndian = Include(quoted("openpal/serialization/BigEndian.h"), Ordering.openpal)

  val parseError = Include(quoted("ssp21/gen/ParseError.h"), Ordering.enum)
  val formatError = Include(quoted("ssp21/gen/FormatError.h"), Ordering.enum)
  val function = Include(quoted("ssp21/gen/Function.h"), Ordering.enum)

  val message = Include(quoted("ssp21/crypto/IMessage.h"), Ordering.crypto)
  val messageField = Include(quoted("ssp21/crypto/IMessageField.h"), Ordering.crypto)


  val seqTypes = Include(quoted("ssp21/crypto/SequenceTypes.h"), Ordering.crypto)
  val msgFormatter = Include(quoted("ssp21/crypto/MessageFormatter.h"), Ordering.crypto)
  val formatResult = Include(quoted("ssp21/crypto/FormatResult.h"), Ordering.crypto)
  val msgPrinter = Include(quoted("ssp21/crypto/IMessagePrinter.h"), Ordering.crypto)
  val msgPrinting = Include(quoted("ssp21/crypto/MessagePrinting.h"), Ordering.crypto)
  val flagsPrinting = Include(quoted("ssp21/crypto/FlagsPrinting.h"), Ordering.crypto)
  val msgParser = Include(quoted("ssp21/crypto/MessageParser.h"), Ordering.crypto)


  def message(className: String) = Include(quoted("ssp21/msg/%s.h".format(className)), Ordering.msg)

  def sort(lhs: Include, rhs: Include): Boolean = {
    if (lhs.order == rhs.order) {
      lhs.file.length < rhs.file.length
    }
    else {
      lhs.order < rhs.order
    }
  }

  def lines(lines: Seq[Include]): Iterator[String] = {
    lines.toSet.toList.sortWith(sort).map(_.line).flatten.toIterator
  }
}

