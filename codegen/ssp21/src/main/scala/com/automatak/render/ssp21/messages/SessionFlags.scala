package com.automatak.render.ssp21.messages

object SessionFlags extends Bitfield {

  def name = "SessionFlags"

  def bits = List(
    Bit("fir", true),
    Bit("fin", true)
  )
}
