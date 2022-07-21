namespace PLU_PRINTER
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.BtnBuscarPuertos = new System.Windows.Forms.Button();
            this.BtnConectar = new System.Windows.Forms.Button();
            this.BtnEnviarDatos = new System.Windows.Forms.Button();
            this.CboPuertos = new System.Windows.Forms.ComboBox();
            this.LblDatosIngreso = new System.Windows.Forms.Label();
            this.TxtDatos_a_Enviar = new System.Windows.Forms.TextBox();
            this.TxtDatosRecibidos = new System.Windows.Forms.TextBox();
            this.SpPuertos = new System.IO.Ports.SerialPort(this.components);
            this.txtPieDePagina = new System.Windows.Forms.TextBox();
            this.lblPiedepagina = new System.Windows.Forms.Label();
            this.lblENCABEZADDO = new System.Windows.Forms.Label();
            this.btnConfigurarBascula = new System.Windows.Forms.Button();
            this.directorySearcher1 = new System.DirectoryServices.DirectorySearcher();
            this.btnAsignarImagen = new System.Windows.Forms.Button();
            this.lblImageName = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // BtnBuscarPuertos
            // 
            this.BtnBuscarPuertos.Location = new System.Drawing.Point(15, 12);
            this.BtnBuscarPuertos.Name = "BtnBuscarPuertos";
            this.BtnBuscarPuertos.Size = new System.Drawing.Size(78, 23);
            this.BtnBuscarPuertos.TabIndex = 0;
            this.BtnBuscarPuertos.Text = "BUSCAR PUERTOS";
            this.BtnBuscarPuertos.UseVisualStyleBackColor = true;
            this.BtnBuscarPuertos.Click += new System.EventHandler(this.BtnBuscarPuertos_Click);
            // 
            // BtnConectar
            // 
            this.BtnConectar.Location = new System.Drawing.Point(247, 12);
            this.BtnConectar.Name = "BtnConectar";
            this.BtnConectar.Size = new System.Drawing.Size(117, 23);
            this.BtnConectar.TabIndex = 1;
            this.BtnConectar.Text = "CONECTAR";
            this.BtnConectar.UseVisualStyleBackColor = true;
            this.BtnConectar.Click += new System.EventHandler(this.BtnConectar_Click);
            // 
            // BtnEnviarDatos
            // 
            this.BtnEnviarDatos.Location = new System.Drawing.Point(18, 399);
            this.BtnEnviarDatos.Name = "BtnEnviarDatos";
            this.BtnEnviarDatos.Size = new System.Drawing.Size(123, 23);
            this.BtnEnviarDatos.TabIndex = 2;
            this.BtnEnviarDatos.Text = "TEST IMPRESOR";
            this.BtnEnviarDatos.UseVisualStyleBackColor = true;
            this.BtnEnviarDatos.Click += new System.EventHandler(this.BtnEnviarDatos_Click);
            // 
            // CboPuertos
            // 
            this.CboPuertos.FormattingEnabled = true;
            this.CboPuertos.Location = new System.Drawing.Point(110, 12);
            this.CboPuertos.Name = "CboPuertos";
            this.CboPuertos.Size = new System.Drawing.Size(121, 21);
            this.CboPuertos.TabIndex = 3;
            // 
            // LblDatosIngreso
            // 
            this.LblDatosIngreso.AutoSize = true;
            this.LblDatosIngreso.Location = new System.Drawing.Point(15, 435);
            this.LblDatosIngreso.Name = "LblDatosIngreso";
            this.LblDatosIngreso.Size = new System.Drawing.Size(105, 13);
            this.LblDatosIngreso.TabIndex = 6;
            this.LblDatosIngreso.Text = "DATOS RECIBIDOS";
            // 
            // TxtDatos_a_Enviar
            // 
            this.TxtDatos_a_Enviar.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TxtDatos_a_Enviar.Location = new System.Drawing.Point(18, 121);
            this.TxtDatos_a_Enviar.Multiline = true;
            this.TxtDatos_a_Enviar.Name = "TxtDatos_a_Enviar";
            this.TxtDatos_a_Enviar.Size = new System.Drawing.Size(346, 121);
            this.TxtDatos_a_Enviar.TabIndex = 7;
            // 
            // TxtDatosRecibidos
            // 
            this.TxtDatosRecibidos.Location = new System.Drawing.Point(15, 451);
            this.TxtDatosRecibidos.Multiline = true;
            this.TxtDatosRecibidos.Name = "TxtDatosRecibidos";
            this.TxtDatosRecibidos.Size = new System.Drawing.Size(346, 54);
            this.TxtDatosRecibidos.TabIndex = 8;
            // 
            // SpPuertos
            // 
            this.SpPuertos.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.DatoRecibido);
            // 
            // txtPieDePagina
            // 
            this.txtPieDePagina.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtPieDePagina.Location = new System.Drawing.Point(18, 275);
            this.txtPieDePagina.Multiline = true;
            this.txtPieDePagina.Name = "txtPieDePagina";
            this.txtPieDePagina.Size = new System.Drawing.Size(346, 107);
            this.txtPieDePagina.TabIndex = 9;
            // 
            // lblPiedepagina
            // 
            this.lblPiedepagina.AutoSize = true;
            this.lblPiedepagina.Location = new System.Drawing.Point(15, 259);
            this.lblPiedepagina.Name = "lblPiedepagina";
            this.lblPiedepagina.Size = new System.Drawing.Size(85, 13);
            this.lblPiedepagina.TabIndex = 10;
            this.lblPiedepagina.Text = "PIE DE PAGINA";
            // 
            // lblENCABEZADDO
            // 
            this.lblENCABEZADDO.AutoSize = true;
            this.lblENCABEZADDO.Location = new System.Drawing.Point(15, 105);
            this.lblENCABEZADDO.Name = "lblENCABEZADDO";
            this.lblENCABEZADDO.Size = new System.Drawing.Size(80, 13);
            this.lblENCABEZADDO.TabIndex = 11;
            this.lblENCABEZADDO.Text = "ENCABEZADO";
            this.lblENCABEZADDO.Click += new System.EventHandler(this.label1_Click);
            // 
            // btnConfigurarBascula
            // 
            this.btnConfigurarBascula.Location = new System.Drawing.Point(174, 399);
            this.btnConfigurarBascula.Name = "btnConfigurarBascula";
            this.btnConfigurarBascula.Size = new System.Drawing.Size(146, 23);
            this.btnConfigurarBascula.TabIndex = 12;
            this.btnConfigurarBascula.Text = "CONFIGURAR BASCULA";
            this.btnConfigurarBascula.UseVisualStyleBackColor = true;
            this.btnConfigurarBascula.Click += new System.EventHandler(this.btnConfigurarBascula_Click);
            // 
            // directorySearcher1
            // 
            this.directorySearcher1.ClientTimeout = System.TimeSpan.Parse("-00:00:01");
            this.directorySearcher1.ServerPageTimeLimit = System.TimeSpan.Parse("-00:00:01");
            this.directorySearcher1.ServerTimeLimit = System.TimeSpan.Parse("-00:00:01");
            // 
            // btnAsignarImagen
            // 
            this.btnAsignarImagen.Location = new System.Drawing.Point(18, 59);
            this.btnAsignarImagen.Name = "btnAsignarImagen";
            this.btnAsignarImagen.Size = new System.Drawing.Size(102, 24);
            this.btnAsignarImagen.TabIndex = 13;
            this.btnAsignarImagen.Text = "Asignar Imagen";
            this.btnAsignarImagen.UseVisualStyleBackColor = true;
            this.btnAsignarImagen.Click += new System.EventHandler(this.btnAsignarImagen_Click);
            // 
            // lblImageName
            // 
            this.lblImageName.AutoSize = true;
            this.lblImageName.Location = new System.Drawing.Point(138, 69);
            this.lblImageName.Name = "lblImageName";
            this.lblImageName.Size = new System.Drawing.Size(13, 13);
            this.lblImageName.TabIndex = 14;
            this.lblImageName.Text = "  ";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 512);
            this.Controls.Add(this.lblImageName);
            this.Controls.Add(this.btnAsignarImagen);
            this.Controls.Add(this.btnConfigurarBascula);
            this.Controls.Add(this.lblENCABEZADDO);
            this.Controls.Add(this.lblPiedepagina);
            this.Controls.Add(this.txtPieDePagina);
            this.Controls.Add(this.TxtDatosRecibidos);
            this.Controls.Add(this.TxtDatos_a_Enviar);
            this.Controls.Add(this.LblDatosIngreso);
            this.Controls.Add(this.CboPuertos);
            this.Controls.Add(this.BtnEnviarDatos);
            this.Controls.Add(this.BtnConectar);
            this.Controls.Add(this.BtnBuscarPuertos);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button BtnBuscarPuertos;
        private System.Windows.Forms.Button BtnConectar;
        private System.Windows.Forms.Button BtnEnviarDatos;
        private System.Windows.Forms.ComboBox CboPuertos;
        private System.Windows.Forms.Label LblDatosIngreso;
        private System.Windows.Forms.TextBox TxtDatos_a_Enviar;
        private System.Windows.Forms.TextBox TxtDatosRecibidos;
        private System.IO.Ports.SerialPort SpPuertos;
        private System.Windows.Forms.TextBox txtPieDePagina;
        private System.Windows.Forms.Label lblPiedepagina;
        private System.Windows.Forms.Label lblENCABEZADDO;
        private System.Windows.Forms.Button btnConfigurarBascula;
        private System.DirectoryServices.DirectorySearcher directorySearcher1;
        private System.Windows.Forms.Button btnAsignarImagen;
        private System.Windows.Forms.Label lblImageName;
    }
}

