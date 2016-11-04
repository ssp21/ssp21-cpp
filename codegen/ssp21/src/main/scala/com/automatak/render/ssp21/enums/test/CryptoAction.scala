/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.test

import com.automatak.render._

object CryptoAction {

  private val comments = List("Enumerates possible cryptographic actions")

  def apply(): EnumModel = EnumModel(
    "CryptoAction",
    "crypto_actions",
    comments,
    EnumModel.UInt8,
    codes,
    None,
    None,
    Hex
  )

  private val codes = List(
    EnumValue("secure_equals", 0, ""),
    EnumValue("hash_sha256", 1, ""),
    EnumValue("hmac_sha256", 2, ""),
    EnumValue("gen_keypair_x25519", 3, ""),
    EnumValue("dh_x25519", 4, "")
  )

}



