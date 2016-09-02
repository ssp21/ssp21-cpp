/**
  * License TBD
  */
package com.automatak.render.ssp21

import java.nio.file.{Files, Paths}

object Generate {

  val basePath = "../cpp/libs";

  val ssp21GenHeaderPath = Paths.get(basePath, "/include/ssp21/gen")
  val ssp21GenImplPath = Paths.get(basePath, "/src/ssp21/gen")

  val ssp21MsgHeaderPath = Paths.get(basePath, "/include/ssp21/msg")
  val ssp21MsgImplPath = Paths.get(basePath, "/src/ssp21/msg")

  val paths = List(ssp21GenHeaderPath, ssp21GenImplPath, ssp21MsgHeaderPath, ssp21MsgImplPath)

  def main(args: Array[String]): Unit = {

    paths.foreach(p => Files.createDirectories(p))

    Structs.files.foreach(_.write(ssp21MsgHeaderPath, ssp21MsgImplPath))
    Bitfields.files.foreach(_.write(ssp21MsgHeaderPath, ssp21MsgImplPath))
    AllEnums.files.foreach(_.write(ssp21GenHeaderPath, ssp21GenImplPath))
    AllMessages.files.foreach(_.write(ssp21MsgHeaderPath, ssp21MsgImplPath))

  }

}
