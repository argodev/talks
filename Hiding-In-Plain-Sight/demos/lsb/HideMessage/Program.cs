using log4net.Config;
using System;
using System.Collections;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;

namespace HideMessage
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

            HideMessage(options);
            
            stopWatch.Stop();
            log.Info(string.Format("{0} elapsed", stopWatch.Elapsed));
            Console.WriteLine("");
            Environment.Exit(0);
        }

        private static void HideMessage(Options options)
        {
            var image = new Bitmap(options.InputPath);
            var messageBits = new BitArray(Encoding.UTF8.GetBytes(options.Message));
            log.InfoFormat("Message Length: {0} bits", messageBits.Length);

            var maxBits = (image.Height * image.Width);
            log.InfoFormat("Max message size: {0} bits", maxBits);

            // write a cheesy header
            int messageLength = messageBits.Length;

            BitArray header = new BitArray(BitConverter.GetBytes(messageLength));
            log.InfoFormat("Header Size: {0} bits", header.Length);

            WriteDataToImage(header, ref image, 0);
            WriteDataToImage(messageBits, ref image, header.Length);

            image.Save(options.OutputPath, ImageFormat.Png);
        }

        private static void WriteDataToImage(BitArray messageData, ref Bitmap carrierImage, int offset)
        {
            // sanity check
            if (messageData.Length + offset > carrierImage.Width * carrierImage.Height)
            {
                // message is too long... handle error appropriately
                return; // not this way
            }

            // ok, still here - let's mod the image with the message
            for (int i = offset; i < messageData.Length + offset; i++)
            {
                // calculate the x,y into the image
                var y = i == 0 ? 0 : i / carrierImage.Width;
                var x = i == 0 ? 0 : i % carrierImage.Width;

                var currentPixel = carrierImage.GetPixel(x, y);

                // let's update the LSB the expensive way
                var bitArray = new BitArray(new byte[] { currentPixel.R });

                bitArray[0] = messageData[i - offset];

                // put the data back into the pixel and then picture
                var newColor = Color.FromArgb(
                    currentPixel.A, 
                    ConvertToByte(bitArray), 
                    currentPixel.G, 
                    currentPixel.B);

                carrierImage.SetPixel(x, y, newColor);
            }
        }

        private static void PrettyPrintBitArray(BitArray rawData, bool reverse)
        {
            StringBuilder byteString = new StringBuilder();
            for (int i = 0; i < rawData.Length; i++)
            {
                if (i % 4 == 0)
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

        private static string ByteToString(byte myByte)
        {
            var bitArray = new BitArray(new byte[] { myByte });
            StringBuilder byteString = new StringBuilder();
            for (int i = 0; i < bitArray.Length; i++)
            {
                byteString.Append(bitArray[i] ? "1" : "0");
            }

            return byteString.ToString();
        }

        private static byte ConvertToByte(BitArray bits)
        {
            if (bits.Count != 8)
            {
                throw new ArgumentException("bits");
            }
            byte[] bytes = new byte[1];
            bits.CopyTo(bytes, 0);
            return bytes[0];
        }

        private static string BuildLongMessageString()
        {
            StringBuilder msgbuilder = new StringBuilder();

            for (int i = 0; i < 315; i++)
            {
                msgbuilder.AppendLine("Lorem ipsum dolor sit amet, consectetur adipiscing elit. In consectetur laoreet est, sit amet eleifend lectus luctus eu. Praesent non tristique magna, et porta metus. Cras aliquet, sem sed feugiat elementum, risus nisl tempus quam, sed porta sem ante vel justo. Vestibulum felis leo, placerat quis justo blandit, ultricies malesuada arcu. Mauris velit dolor, interdum eget porttitor ac, egestas vel eros. Nam sit amet elit enim. Suspendisse ipsum elit, tempor nec vehicula id, aliquet eget eros. Cras fringilla euismod lectus, sit amet fermentum ipsum molestie et. Vestibulum at mollis nibh. Morbi ut mauris risus. Mauris libero dolor, aliquam dictum nibh quis, dignissim interdum enim.");
                msgbuilder.AppendLine("Etiam at felis vitae lectus auctor gravida. Donec consequat nisl vitae elit sollicitudin hendrerit. Nunc porta vestibulum vestibulum. Fusce eu velit pretium, sagittis ligula ut, pellentesque arcu. In hac habitasse platea dictumst. Nullam viverra justo eget ultricies aliquet. Nam egestas semper sem, vitae laoreet leo. Fusce cursus vehicula enim, sagittis consectetur justo dapibus eget. Vivamus pulvinar, ligula et rhoncus bibendum, orci nunc mattis metus, sed feugiat erat nibh et magna. Vivamus vitae euismod urna.");
                msgbuilder.AppendLine("Morbi vestibulum odio eu orci luctus, in aliquet justo placerat. Vestibulum convallis lorem est, in vestibulum elit scelerisque at. Curabitur porta luctus sem. Ut in rutrum sem, a feugiat metus. Morbi a sollicitudin justo. Pellentesque ullamcorper elementum malesuada. Curabitur euismod elementum velit, ut suscipit massa vestibulum volutpat. Sed ut mauris eget odio fringilla congue sed id dolor. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Praesent pharetra, est nec tempor rhoncus, lorem sem tincidunt purus, quis gravida lorem erat quis augue. Vestibulum erat nisi, condimentum sed nibh at, porttitor scelerisque odio. Nunc eget lorem sed mauris consequat dictum et non ipsum. Cras auctor augue dui, eget congue tellus tincidunt sed.");
                msgbuilder.AppendLine("Donec accumsan posuere vulputate. Sed vitae massa facilisis, condimentum erat nec, ultrices est. Phasellus nec euismod dolor, vitae venenatis ipsum. Etiam mattis et urna in pellentesque. Integer ut magna mattis, varius nisl at, aliquet massa. Curabitur malesuada in mauris at rutrum. Morbi sollicitudin interdum sem, ut porttitor tortor vehicula sit amet. Nam ut varius urna, sit amet tincidunt est. Nulla elementum congue nibh, vitae auctor tellus fringilla sed. Quisque ullamcorper massa nulla, faucibus vestibulum risus venenatis ac.");
                msgbuilder.AppendLine("Aliquam vel venenatis mauris. Nam non risus vitae dolor lacinia porta eget vestibulum lacus. Nulla cursus eget lorem vitae adipiscing. Vivamus commodo semper vehicula. Nam eget odio ullamcorper, aliquet nulla nec, posuere dolor. Nam quis mollis diam. Nullam ac venenatis sapien, rutrum laoreet diam. Nulla iaculis nibh neque, ac euismod ipsum commodo et. Etiam id turpis nisl. Quisque pulvinar tortor quis magna fringilla placerat quis eget nisl. Pellentesque faucibus volutpat ornare. Sed sed enim euismod, malesuada nunc eu, consectetur diam. Praesent gravida nisl in vulputate tincidunt.");
            }

            return msgbuilder.ToString();
        }
    }
}
