package com.automatak.render.ssp21

import com.automatak.render.cpp._

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
  val formatError = Include(quoted("ssp21/gen/FormatError.h"), Ordering.ssp21)

  def lines(i : Seq[Include]) : Iterator[String] = {
    val sorted : List[Include] = i.toSet.toList.sortWith((lhs, rhs) => lhs.order > rhs.order)
    sorted.map(_.line).toIterator
  }
}

