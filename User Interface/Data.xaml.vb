'**************************************************************************************
'Easy EBSD Data Analyzer

'*	Author: Anil Kumar
'*	email: aniliitb10@gmail.com
'*	file: Data.xaml.vb
'*	date added: July 30, 2015
'*	Copyright © 2015 Anil Kumar

'*	This file is part of Easy EBSD Data Analyzer.

'    Easy EBSD Data Analyzer is free software: you can redistribute it and/or modify
'    it under the terms of the GNU General Public License as published by
'    the Free Software Foundation, either version 3 of the License, or
'    any later version.

'    Easy EBSD Data Analyzer is distributed in the hope that it will be useful,
'    but WITHOUT ANY WARRANTY; without even the implied warranty of
'    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'    GNU General Public License for more details.

'    You should have received a copy of the GNU General Public License
'    along with Easy EBSD Data Analyzer.  If not, see <http://www.gnu.org/licenses/>.

'**************************************************************************************


Class Data
    
    Dim checkValues(6) As Integer
    Dim ci As Double = 0.1
    Dim angle1 As Double = 5.0
    Dim angle2 As Double = 5.0
    Dim angle3 As Double = 5.0
    Dim gsize As Integer = 2
    Dim n_bins As Integer = 20
    Dim error_counter As Integer = 0
    Dim cb_advBinningIndex As Integer = 0

    Public Sub initialize(i1 As Integer, i2 As Integer, i3 As Integer, i4 As Integer, i5 As Integer, i6 As Integer, i7 As Integer)
        checkValues(0) = i1
        checkValues(1) = i2
        checkValues(2) = i3
        checkValues(3) = i4
        checkValues(4) = i5
        checkValues(5) = i6
        checkValues(6) = i7


        'intializing the values
        dataBox1.Text = ci.ToString
        dataBox2.Text = angle1.ToString
        dataBox3.Text = angle2.ToString
        dataBox4.Text = angle3.ToString
        dataBox5.Text = gsize.ToString
        dataBox6.Text = n_bins.ToString


        'limiting the editibilities of TextBoxes
        If (checkValues(2) = 0) Then       'twin boundaries
            dataBox3.IsEnabled = False
            dataBox4.IsEnabled = False
        End If

        If ((checkValues(4) = 0) And (checkValues(5) = 0) And (checkValues(6) = 0)) Then
            dataBox5.IsEnabled = False
            dataBox6.IsEnabled = False
            cb_advBinning.IsEnabled = False
        End If

        If (checkValues(3) = 1) Then
            dataBox6.IsEnabled = True
            cb_advBinning.IsEnabled = True
        End If

    End Sub



    Private Sub dataButton_Click(sender As Object, e As RoutedEventArgs) Handles dataButton.Click
   

        error_counter = 0 'initializing the error counter every time dataButton is clicked!

        If String.IsNullOrWhiteSpace(dataBox1.Text) Then
            'go with the defaut value
        ElseIf IsNumeric(dataBox1.Text) Then
            ci = CDbl(dataBox1.Text)
            If (ci < 0 Or ci >= 1) Then
                messageBox("ci should be between 0 and 1")
                ci = 0.1
            End If

        Else
            messageBox("Unidentified input for ci")
            'Exit Sub
        End If

        If String.IsNullOrWhiteSpace(dataBox2.Text) Then
            'go with the defaut value
        ElseIf IsNumeric(dataBox2.Text) Then
            angle1 = CDbl(dataBox2.Text)
            If (angle1 <= 0 Or angle1 >= 180) Then
                messageBox("Misorientation should be between 0 and 180")
                angle1 = 5.0
            End If
        Else
            messageBox("Unidentified input for Misorientation")
            'Exit Sub
        End If

        If String.IsNullOrWhiteSpace(dataBox3.Text) Then
            'go with the defaut value
        ElseIf IsNumeric(dataBox3.Text) Then
            angle2 = CDbl(dataBox3.Text)
            If (angle2 <= 0 Or angle2 >= 180) Then
                messageBox("Tolerance for Misorientation should be between 0 and 180")
                angle2 = 5.0
            End If
        Else
            messageBox("Unidentified input for Tolerance for Misorientation")
            'Exit Sub
        End If

        If String.IsNullOrWhiteSpace(dataBox4.Text) Then
            'go with the defaut value
        ElseIf IsNumeric(dataBox4.Text) Then
            angle3 = CDbl(dataBox4.Text)
            If (angle3 <= 0 Or angle3 >= 180) Then
                messageBox("Tolerance for K1 plane should be between 0 and 180")
                angle3 = 5.0
            End If
        Else
            messageBox("Unidentified input for Tolerance for K1 plane")
            'Exit Sub
        End If

        If String.IsNullOrWhiteSpace(dataBox5.Text) Then
            'go with the defaut value
        ElseIf IsNumeric(dataBox5.Text) Then
            gsize = CInt(dataBox5.Text)
            If (gsize < 2) Then
                messageBox("The minimum grain size should be 2")
                gsize = 2
            End If
        Else
            messageBox("Unidenified input for grain size")
            'Exit Sub
        End If

        If String.IsNullOrWhiteSpace(dataBox6.Text) Then
            'go with the defaut value
        ElseIf IsNumeric(dataBox6.Text) Then
            n_bins = CInt(dataBox6.Text)
            If (n_bins < 2) Then
                messageBox("The minimum number of bins shouls be 2")
                n_bins = 2
            End If
        Else
            messageBox("Unidentified input for number of bins")
            'Exit Sub
        End If


        If (error_counter > 0) Then
            'nothing happens, user is asked to edit the input
        ElseIf (error_counter = 0) Then

            Dim file As System.IO.StreamWriter
            file = My.Computer.FileSystem.OpenTextFileWriter("response.txt", False, System.Text.Encoding.ASCII)  'false overwrites the content

            file.WriteLine(checkValues(0).ToString + " " + angle1.ToString + " " + "0.0" + " " + "0.0" + " " + ci.ToString)
            file.WriteLine(checkValues(1).ToString + " " + angle1.ToString + " " + "0.0" + " " + "0.0" + " " + ci.ToString)
            file.WriteLine(checkValues(2).ToString + " " + angle1.ToString + " " + angle2.ToString + " " + angle3.ToString + " " + ci.ToString)
            file.WriteLine(checkValues(3).ToString + " " + angle1.ToString + " " + "0.0" + " " + n_bins.ToString + " " + ci.ToString)
            file.WriteLine(checkValues(4).ToString + " " + angle1.ToString + " " + gsize.ToString + " " + n_bins.ToString + " " + ci.ToString)
            file.WriteLine(checkValues(5).ToString + " " + angle1.ToString + " " + gsize.ToString + " " + n_bins.ToString + " " + ci.ToString)
            file.WriteLine(checkValues(6).ToString + " " + angle1.ToString + " " + gsize.ToString + " " + n_bins.ToString + " " + ci.ToString)

            file.Close()

            'MsgBox("All values have been successfully taken" + Environment.NewLine _
            '       + "This interface will close now!", MsgBoxStyle.Information)
            'Process.Start("C:\Users\Anil Kumar\Documents\Visual Studio 2013\Projects\CandC++\Release\CandC++.exe")
            ''Process.Start("MainApplication.exe")
            'Application.Current.Shutdown()
            If (cb_advBinningIndex = 1) Then
                Dim advBin As AdvancedBinning = New AdvancedBinning()
                Me.NavigationService.Navigate(advBin)
                advBin.initialize(checkValues(3), checkValues(4), checkValues(5), checkValues(6), n_bins)

            Else
                MsgBox("All values have been successfully taken" + Environment.NewLine _
                      + "This interface will close now!", MsgBoxStyle.Information)

                Try
                    Process.Start("Main Application.exe")
                Catch ex As Exception
                    MsgBox("Error in opening main application file:" & Environment.NewLine & ex.Message, MsgBoxStyle.Critical)
                End Try
                'Process.Start("C:\Users\Anil Kumar\Documents\Visual Studio 2013\Projects\CandC++\Release\CandC++.exe")
                'Process.Start("CandC++.exe")
                Application.Current.Shutdown()
            End If


        End If

    End Sub


    Public Sub messageBox(message As String)
        MsgBox(message + Environment.NewLine + "Please modify the input to proceed", MsgBoxStyle.Information)
        error_counter += 1
    End Sub


    Private Sub cb_advBinning_checked(sender As Object, e As RoutedEventArgs) Handles cb_advBinning.Checked
        cb_advBinningIndex = 1
        dataButton.Content = "Next"
    End Sub

    Private Sub cb_advBinning_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_advBinning.Unchecked
        cb_advBinningIndex = 0
        dataButton.Content = "Finish"
    End Sub
End Class
