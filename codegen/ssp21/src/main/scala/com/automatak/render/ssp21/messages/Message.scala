package com.automatak.render.ssp21.messages

trait Message {
  def name : String
  def fields : List[Field]
}
