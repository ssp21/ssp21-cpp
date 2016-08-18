package com.automatak.render.ssp21

import java.nio.file.Path

import com.automatak.render._
import com.automatak.render.cpp.CppIndentation


trait CppFiles {

  def mainClassName : String
  def header(implicit i : Indentation) : Iterator[String]
  def impl(implicit i : Indentation) : Iterator[String]

  def hasImpl : Boolean = true

  def headerFileName : String = mainClassName + ".h"
  def implFileName : String = mainClassName + ".cpp"

  def write(incDirectory: Path, implDirectory: Path): Unit = {

    implicit val indent = CppIndentation()

    val headerPath = incDirectory.resolve(headerFileName)
    val implPath = implDirectory.resolve(implFileName)

    writeTo(headerPath)(header)
    println("Wrote: " + headerPath)

    if(hasImpl) {
      writeTo(implPath)(impl)
      println("Wrote: " + implPath)
    }
  }
}
