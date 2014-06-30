using CommandLine;
using CommandLine.Text;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HideMessage
{
    public class Options
    {
        [Option('i', "input", HelpText = "The file system path to the image you would like to use " +
            "to hide your message.", Required = true)]
        public string InputPath { get; set; }

        [Option('m', "message", HelpText = "The message you want to embed in the image.", Required = true)]
        public string Message { get; set; }

        [Option('o', "output", HelpText = "The file system path to the modified image.", Required = true)]
        public string OutputPath { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            return HelpText.AutoBuild(this, (HelpText current) => 
                HelpText.DefaultParsingErrorsHandler(this, current));
        }
    }
}
