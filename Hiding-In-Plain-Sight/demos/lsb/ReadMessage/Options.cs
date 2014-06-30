using CommandLine;
using CommandLine.Text;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReadMessage
{
    public class Options
    {
        [Option('i', "input", HelpText = "The file system path to the image you would like to read " +
            "a message from.", Required = true)]
        public string InputPath { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            return HelpText.AutoBuild(this, (HelpText current) =>
                HelpText.DefaultParsingErrorsHandler(this, current));
        }

    }
}
