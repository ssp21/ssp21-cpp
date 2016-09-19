/**
  * License TBD
  */
package com.automatak.render.ssp21



object Generate {

  def getValue(c : Char): Int = c match {
    case _ if(c >= 'A' && c <= 'Z') => (c.toByte  - 65).toByte  // base64 0 -> 25
    case _ if(c >= 'a' && c <= 'z') => (c.toByte  - 71).toByte  // base64 26 -> 51
    case _ if(c >= '0' && c <= '9') => (c.toByte  + 4).toByte   // base64 52 -> 61
    case '+' => 62
    case '/' => 63
    case _ => 255
  }


  def main(args: Array[String]): Unit = {

    val values = (0 to 255).map(b => getValue(b.toChar)).grouped(16)

    def printGroup(group : Seq[Int]): Unit = {
      println(group.map(i => "0x%02x".format(i)).mkString(", "))
    }

    for {
      group <- values
    } printGroup(group)

  }

}
