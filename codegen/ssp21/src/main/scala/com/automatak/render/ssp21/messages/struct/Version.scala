/**
 * License TBD
 */
package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages._

object Version extends Struct {

  override def name: String = "Version"

  override def fields: List[Field] = List(
    U16("major"),
    U16("minor")
  )

  override def public: Boolean = false

}
