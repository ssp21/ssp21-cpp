package com.automatak.render.ssp21

import com.automatak.render._
import com.automatak.render.cpp._

object Ordering {
  val system = 0
  val openpal = 1
  val ssp21 = 2
}

case class Include(file: String, order: Int) {
  def line : Iterator[String] = include(file)
}

object Includes {

  val cstdint = Include("<cstdint>", Ordering.system)

  val rslice = Include(quoted("openpal/container/RSlice.h"), Ordering.openpal)
  val wslice = Include(quoted("openpal/container/WSlice.h"), Ordering.openpal)
  val uncopyable = Include(quoted("openpal/util/Uncopyable.h"), Ordering.openpal)

  val seqTypes = Include(quoted("ssp21/SequenceTypes.h"), Ordering.ssp21)
  val parseError = Include(quoted("ssp21/gen/ParseError.h"), Ordering.ssp21)
  val formatError = Include(quoted("ssp21/gen/FormatError.h"), Ordering.ssp21)
  val function = Include(quoted("ssp21/gen/Function.h"), Ordering.ssp21)
  val msgParser = Include(quoted("ssp21/MessageParser.h"), Ordering.ssp21)
  val msgFormatter = Include(quoted("ssp21/MessageFormatter.h"), Ordering.ssp21)
  val formatResult = Include(quoted("ssp21/FormatResult.h"), Ordering.ssp21)

  def sort(lhs : Include, rhs : Include) : Boolean = {
    if(lhs.order == rhs.order) {
      lhs.file.length < rhs.file.length
    }
    else {
      lhs.order < rhs.order
    }
  }

  def lines(lines : Seq[Include]) : Iterator[String] = {
    lines.toSet.toList.sortWith(sort).map(_.line).flatten.toIterator
  }
}

