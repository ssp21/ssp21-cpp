/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.cpp

import com.automatak.render._

class SwitchModelRenderer[A](fCase: A => String)(fAction: A => String) {

  def render(nonDefaults: List[A], default: A)(implicit indent: Indentation): Iterator[String] = {

    def switch = "switch(arg)".iter

    def nonDefaultCases: Iterator[String] = nonDefaults.toIterator.map { c =>
      "case(%s):".format(fCase(c)).iter ++ indent {
        "return %s;".format(fAction(c)).iter
      }
    }.flatten

    def defaultCase: Iterator[String] = {
      "default:".iter ++ indent {
        "return %s;".format(fAction(default)).iter
      }
    }

    switch ++ bracket {
      nonDefaultCases ++
        defaultCase
    }
  }
}
