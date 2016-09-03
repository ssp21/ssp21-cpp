package com.automatak.render.ssp21.messages.generators

import com.automatak.render._
import com.automatak.render.ssp21.messages.Message
import com.automatak.render.ssp21.{Include, Includes}


final case class MessageGenerator(msg: Message) extends StructGenerator(msg) {

  override def interfaces: String = "public IMessage, private openpal::Uncopyable"

  override def headerIncludes: List[Include] = List(Includes.imessage, Includes.function)

  override def outputReadWritePrint: Boolean = false

  override def extraHeaderConstants: Iterator[String] = Iterator(
    "static const Function function = Function::%s;".format(msg.function.name)
  )

  override def extraHeaderSignatures: Iterator[String] = Iterator(
    "virtual ParseError read(openpal::RSlice input) override;",
    "virtual FormatResult write(openpal::WSlice output) const override;",
    "virtual void print(IMessagePrinter& printer) const override;"
  ) ++ space

  override def extraImplFunctions(implicit indent: Indentation): Iterator[String] = {

    def read = {
      "ParseError %s::read(openpal::RSlice input)".format(msg.name).iter ++ bracket {
        "auto read_fields = [this](openpal::RSlice& input) -> ParseError ".iter ++ bracketSemiColon {
          readInternals
        } ++ space ++
        "return MessageParser::read_message(input, Function::%s, read_fields);".format(msg.function.name).iter
      }
    }

    def write = {
      "FormatResult %s::write(openpal::WSlice output) const".format(msg.name).iter ++ bracket {
        "auto write_fields = [this](openpal::WSlice& output) -> FormatError ".iter ++ bracketSemiColon {
          writeInternals
        } ++ space ++
          "return MessageFormatter::write_message(output, Function::%s, write_fields);".format(msg.function.name).iter
      }
    }

    def print = {
      "void %s::print(IMessagePrinter& printer) const".format(msg.name).iter ++ bracket {
        printInternals
      }
    }

    space ++ read ++ space ++ write ++ print ++ space
  }


}
