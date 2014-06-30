using log4net.Config;
using System;
using System.Collections;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;

namespace ReadMessage
{
    class Program
    {
        private static readonly log4net.ILog log =
            log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

        static void Main(string[] args)
        {
            XmlConfigurator.Configure();
            Stopwatch stopWatch = new Stopwatch();
            stopWatch.Start();

            var options = new Options();
            var parser = new CommandLine.Parser(with => with.HelpWriter = Console.Error);

            if (!parser.ParseArguments(args, options))
            {
                Environment.Exit(1);
            }

            ReadMessage(options);

            stopWatch.Stop();
            log.Info(string.Format("{0} elapsed", stopWatch.Elapsed));
            Console.WriteLine("");
            Environment.Exit(0);
        }

        private static void ReadMessage(Options options)
        {
            var image = new Bitmap(options.InputPath);

            // read the header
            int length = ReadIntDataFromImage(image, 0, 32);
            log.InfoFormat("Message length: {0} bits", length);

            // read the message
            string message = ReadStringDataFromImage(image, 32, length);

            // write it to the screen
            log.InfoFormat("Message: {0}", message);
        }

        private static int ReadIntDataFromImage(Bitmap carrierImage, int offset, int length)
        {
            BitArray rawData = ReadDataFromImage(carrierImage, offset, length);
            // PrettyPrintBitArray(rawData, false);
            byte[] bytes = new byte[4];
            rawData.CopyTo(bytes, 0);
            return BitConverter.ToInt32(bytes, 0);
        }

        private static string ReadStringDataFromImage(Bitmap carrierImage, int offset, int length)
        {
            BitArray rawData = ReadDataFromImage(carrierImage, offset, length);
            byte[] bytes = new byte[length / 8];
            rawData.CopyTo(bytes, 0);
            return Encoding.UTF8.GetString(bytes);
        }

        private static BitArray ReadDataFromImage(Bitmap carrierImage, int offset, int length)
        {
            // sanity check
            if (length + offset > carrierImage.Width * carrierImage.Height)
            {
                // read request is too long... handle error appropriately
                return null; // probably not this way
            }

            BitArray rawData = new BitArray(length);

            // ok, still here - let's mod the image with the message
            for (int i = offset; i < length + offset; i++)
            {
                // calculate the x,y into the image
                var y = i == 0 ? 0 : i / carrierImage.Width;
                var x = i == 0 ? 0 : i % carrierImage.Width;

                var currentPixel = carrierImage.GetPixel(x, y);

                // let's read the LSB the expensive way
                var bitArray = new BitArray(new byte[] { currentPixel.R });
                rawData[i - offset] = bitArray[0];
            }

            return rawData;
        }

        private static void PrettyPrintBitArray(BitArray rawData, bool reverse)
        {
            BitArray working = rawData;

            StringBuilder byteString = new StringBuilder();
            for (int i = 0; i < working.Length; i++)
            {
                if (i % 4 == 0)
                {
                    byteString.Append(" ");
                }

                if (i % 8 == 0)
                {
                    byteString.Append(" ");
                }

                if (i % 32 == 0)
                {
                    byteString.Append("\n");
                }

                byteString.Append(rawData[i] ? "1" : "0");
            }

            string test = byteString.ToString();

            if (reverse) 
            {
                Console.WriteLine(new string(test.Reverse().ToArray()));
            }
            else
            {
                Console.WriteLine(test);
            }           
        }
    }
}
