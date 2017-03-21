namespace WinFormProject
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.pbSrcImg = new System.Windows.Forms.PictureBox();
            this.pbDstImage = new System.Windows.Forms.PictureBox();
            this.button3 = new System.Windows.Forms.Button();
            this.pictureBoxIpl = new OpenCvSharp.UserInterface.PictureBoxIpl();
            ((System.ComponentModel.ISupportInitialize)(this.pbSrcImg)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbDstImage)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxIpl)).BeginInit();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(33, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(112, 56);
            this.button1.TabIndex = 0;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(152, 12);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(115, 56);
            this.button2.TabIndex = 1;
            this.button2.Text = "button2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // pbSrcImg
            // 
            this.pbSrcImg.Location = new System.Drawing.Point(33, 104);
            this.pbSrcImg.Name = "pbSrcImg";
            this.pbSrcImg.Size = new System.Drawing.Size(350, 500);
            this.pbSrcImg.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pbSrcImg.TabIndex = 2;
            this.pbSrcImg.TabStop = false;
            // 
            // pbDstImage
            // 
            this.pbDstImage.Location = new System.Drawing.Point(405, 104);
            this.pbDstImage.Name = "pbDstImage";
            this.pbDstImage.Size = new System.Drawing.Size(378, 500);
            this.pbDstImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pbDstImage.TabIndex = 3;
            this.pbDstImage.TabStop = false;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(274, 13);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(143, 55);
            this.button3.TabIndex = 4;
            this.button3.Text = "button3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // pictureBoxIpl
            // 
            this.pictureBoxIpl.Location = new System.Drawing.Point(789, 104);
            this.pictureBoxIpl.Name = "pictureBoxIpl";
            this.pictureBoxIpl.Size = new System.Drawing.Size(388, 500);
            this.pictureBoxIpl.TabIndex = 5;
            this.pictureBoxIpl.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1189, 704);
            this.Controls.Add(this.pictureBoxIpl);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.pbDstImage);
            this.Controls.Add(this.pbSrcImg);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pbSrcImg)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbDstImage)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxIpl)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.PictureBox pbSrcImg;
        private System.Windows.Forms.PictureBox pbDstImage;
        
        private System.Windows.Forms.Button button3;
        private OpenCvSharp.UserInterface.PictureBoxIpl pictureBoxIpl;
    }
}

