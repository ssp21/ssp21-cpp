/**
 * License TBD
 */
package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages._

object Version extends Struct {

  override def name: String = "Version"

  override def fields: List[Field] = List(
    U16("major_version"),
    U16("minor_version")
  )

  override def public: Boolean = false

}
