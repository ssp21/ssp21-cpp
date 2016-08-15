/**
 * License TBD
 */
package com.automatak.render.ssp21

import java.nio.file.{Files, Paths}

import com.automatak.render.ssp21.enums.generators.CppEnumGenerator
import com.automatak.render.ssp21.enums.groups.SSP21Enums

object Generate {

  val basePath = "./cpp/libs";
  val ssp21GenHeaderPath = Paths.get(basePath, "include/ssp21/gen")
  val ssp21GenImplPath = Paths.get(basePath, "/src/ssp21/gen")

  def main(args: Array[String]): Unit = {

    Files.createDirectories(ssp21GenHeaderPath)
    Files.createDirectories(ssp21GenImplPath)

    // generate the C++ ssp21 enums
    CppEnumGenerator(SSP21Enums(), "ssp21", "ssp21/gen/%s", ssp21GenHeaderPath, ssp21GenImplPath)

  }

}
