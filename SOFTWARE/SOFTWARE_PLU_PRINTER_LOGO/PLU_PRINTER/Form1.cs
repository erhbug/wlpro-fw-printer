using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO.Ports;


namespace PLU_PRINTER
{
    public partial class Form1 : Form
    {
        private string strBufferIn;
        private string strBufferOut;
        Bitmap bitmap;
        int FlagBMP;
        private byte[] HEADER_PRINT_RASTER_IMAGE = new byte[] { 0x1B, 0X40, 0x1B, 0x4A, 0x05 };
        private byte[] PRINT_RASTER_IMAGE = new byte[] { 0x1D, 0x76, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00 };
        private byte[] Dato = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00 };
        private byte[] Pixel = new byte[] { 0x00 };
        Color color;


        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            strBufferIn = "";
            strBufferOut = "";
            BtnConectar.Enabled = false;
            BtnEnviarDatos.Enabled = false;
            btnConfigurarBascula.Enabled = false;
            btnAsignarImagen.Enabled = false;
            FlagBMP = 0;
        }

        private void BtnBuscarPuertos_Click(object sender, EventArgs e)
        {
            string[] puertosDisponibles = SerialPort.GetPortNames();

            CboPuertos.Items.Clear();

            foreach (string puerto_simple in puertosDisponibles)
            {
                CboPuertos.Items.Add(puerto_simple);

            }
            if (CboPuertos.Items.Count > 0)
            {
                CboPuertos.SelectedIndex = 0;
                MessageBox.Show("SELECCIONAR EL PUERTO DE TRABAJO");
                BtnConectar.Enabled = true;
            }
            else
            {
                MessageBox.Show("NINGUN PUERTO DETECTADO");
                CboPuertos.Items.Clear();
                CboPuertos.Text = "                      ";
                strBufferIn = "";
                strBufferOut = "";
                BtnConectar.Enabled = false;
                BtnEnviarDatos.Enabled = false;
            }
        }

        private void BtnConectar_Click(object sender, EventArgs e)
        {
            try
            {
                if (BtnConectar.Text == "CONECTAR")
                {
                    SpPuertos.BaudRate = 230400;//230400;// 19200;
                    SpPuertos.DataBits = 8;
                    SpPuertos.Parity = Parity.None;
                    SpPuertos.StopBits = StopBits.One;
                    SpPuertos.Handshake = Handshake.None;
                    SpPuertos.PortName = CboPuertos.Text;                  
                try
                    {
                        SpPuertos.Open();
                        BtnConectar.Text = "DESCONECTAR";
                        BtnEnviarDatos.Enabled = true;
                        btnConfigurarBascula.Enabled = true;
                        btnAsignarImagen.Enabled = true;         
                    }
                    catch (Exception exc)
                    {
                        MessageBox.Show(exc.Message.ToString());
                    }
                }
                else if (BtnConectar.Text == "DESCONECTAR")
                {
                    SpPuertos.Close();
                    BtnConectar.Text = "CONECTAR";
                    BtnEnviarDatos.Enabled = false;
                    btnConfigurarBascula.Enabled = false;
                    btnAsignarImagen.Enabled = false;
                }

            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message.ToString());
            }

        }

        private void BtnEnviarDatos_Click(object sender, EventArgs e)
        {
            try
            {
                SpPuertos.DiscardOutBuffer();
                if (FlagBMP==1)
                {                    
                    //SpPuertos.Write(HEADER_PRINT_RASTER_IMAGE, 0, 5);
                    PRINT_RASTER_IMAGE[4] = (byte)((bitmap.Width/8) & 0xFF);
                    PRINT_RASTER_IMAGE[5] = (byte)((bitmap.Width/8) >> 8);
                    PRINT_RASTER_IMAGE[6] = (byte)(bitmap.Height  & 0xFF);
                    PRINT_RASTER_IMAGE[7] = (byte)(bitmap.Height  >> 8);
                   // SpPuertos.Write(PRINT_RASTER_IMAGE, 0, 8);

                    for (int y = 0;y< bitmap.Height;y++) {
                        
                        for (int x = 0; x < bitmap.Width / 8; x++) {
                            Dato[0] = 0x00;
                            for (int c=0;c<8;c++) {
                                Pixel[0] = 0x00;
                                color = bitmap.GetPixel(c + (x * 8), y);
                                if (color.GetBrightness() < 0.5)
                                {
                                    Pixel[0] = 0x01;
                                }
                                Dato[0] |= (byte)(Pixel[0] << (7-c));
                            }
                            SpPuertos.Write(Dato, 0, 1);
                        }                        
                    }
                    Dato[0] = 0x1B;
                    Dato[1] = 0x4A;
                    Dato[2] = 0x18;
                  //  SpPuertos.Write(Dato, 0, 3);

                }
                
                //strBufferOut = TxtDatos_a_Enviar.Text;
               // SpPuertos.Write(strBufferOut);

                //SpPuertos.DiscardOutBuffer();
               // strBufferOut = "\r\nTicket No. 1\r\nPeso / Piezas  PLU PRE.UNIT $    TOTAL $\r\n************************************************\r\n10.525 Kg T-BONE               123.59   1300.78\r\n10.000 Kg TOMATE BOLA           10.50    105.00\r\n 9.500 Kg PECHUGA DE POLLO      40.00    380.00\r\n 9.000 Kg PIERNA Y MUSLO        30.00    270.00\r\n     9 Pz CEBOLLA MORADA         7.00     27.00\r\n    10 Pz SAZONADOR EN CUBO      3.50     35.00\r\n    20 pz MAZAPAN REGIO          9.00    180.00\r\n 1.250 Kg QUESO BLANCO          70.50     88.12\r\n 3.200 Kg AZUCAR                20.00     64.00\r\n 1.000 Kg HUEVO                 30.00     30.00\r\n************************************************\r\nITEMS 10                        TOTAL: $ 2479.90\r\n                             EFECTIVO: $ 3000.00\r\n                               CAMBIO: $  520.10\r\n";
               // SpPuertos.Write(strBufferOut);

               // SpPuertos.DiscardOutBuffer();
               // strBufferOut = txtPieDePagina.Text;
               // SpPuertos.Write(strBufferOut);

                //SpPuertos.DiscardOutBuffer();
               // strBufferOut = "\x01Bi";                
              //  SpPuertos.Write(strBufferOut);

                /*txtPieDePagina*/
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message.ToString());
            }

        }

        private void DatoRecibido(object sender, SerialDataReceivedEventArgs e)
        {
            /*string Data_In = SpPuertos.ReadExisting();
            MessageBox.Show(Data_In);
            TxtDatosRecibidos.Text = Data_In;*/
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void btnAsignarImagen_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            if (openFileDialog.ShowDialog() == DialogResult.OK) {

                string file = openFileDialog.FileName;
                bitmap = new Bitmap(file);
                lblImageName.Text = file;
                FlagBMP = 1;
               // x = bitmap.Width / 8;
               // y= bitmap.Height;
                //bitmap.GetPixel();
            }
        }

        private void btnConfigurarBascula_Click(object sender, EventArgs e)
        { 



                Dato[0] = 0x00;
            for (int c = 0; c < 80*54; c++)
                    {
                       SpPuertos.Write(Dato, 0, 1);
                    }
                    
               
          

        }
    }
}
