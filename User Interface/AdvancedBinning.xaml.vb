'**************************************************************************************
'Easy EBSD Data Analyzer

'*	Author: Anil Kumar
'*	email: aniliitb10@gmail.com
'*	file: AdvancedBinning.xaml.vb
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
Class AdvancedBinning

    Dim KAM_check As Integer = 0
    Dim GAM_check As Integer = 0
    Dim GOS_check As Integer = 0
    Dim GSD_check As Integer = 0
    Dim error_counter As Integer = 0
    Dim bins As Integer = 0
    Dim kam_tb1 As String = "0"
    Dim kam_tb2 As String = "0"
    Dim gam_tb1 As String = "0"
    Dim gam_tb2 As String = "0"
    Dim gos_tb1 As String = "0"
    Dim gos_tb2 As String = "0"
    Dim gsd_tb1 As String = "0"
    Dim gsd_tb2 As String = "0"


    Public Sub initialize(opt3 As Integer, opt4 As Integer, opt5 As Integer, opt6 As Integer, n_bins As Integer)
        If opt3 = 0 Then
            KAM_grid.IsEnabled = False
        Else
            KAM_grid.IsEnabled = True
        End If
        If opt4 = 0 Then
            GAM_grid.IsEnabled = False
        Else
            GAM_grid.IsEnabled = True
        End If
        If opt5 = 0 Then
            GOS_grid.IsEnabled = False
        Else
            GOS_grid.IsEnabled = True
        End If
        If opt6 = 0 Then
            GSD_grid.IsEnabled = False
        Else
            GSD_grid.IsEnabled = True
        End If
        bins = n_bins
    End Sub


    'Now starts KAM
    '========================================= KAM =====================================================================
    Private Sub cb_KAM1_Checked(sender As Object, e As RoutedEventArgs) Handles cb_KAM1.Checked
        cb_KAM2.IsChecked = False
        cb_KAM3.IsChecked = False
        KAM_check = 1
    End Sub

    Private Sub cb_KAM2_Checked(sender As Object, e As RoutedEventArgs) Handles cb_KAM2.Checked
        cb_KAM1.IsChecked = False
        cb_KAM3.IsChecked = False
        KAM_check = 2
    End Sub

    Private Sub cb_KAM3_Checked(sender As Object, e As RoutedEventArgs) Handles cb_KAM3.Checked
        cb_KAM1.IsChecked = False
        cb_KAM2.IsChecked = False
        KAM_check = 3
    End Sub

    Private Sub cb_KAM1_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_KAM1.Unchecked
        KAM_check = 0
    End Sub

    Private Sub cb_KAM2_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_KAM2.Unchecked
        KAM_check = 0
    End Sub
    Private Sub cb_KAM3_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_KAM3.Unchecked
        KAM_check = 0
    End Sub


    'Now starts GAM
    '========================================= GAM =====================================================================
    Private Sub cb_GAM1_Checked(sender As Object, e As RoutedEventArgs) Handles cb_GAM1.Checked
        cb_GAM2.IsChecked = False
        cb_GAM3.IsChecked = False
        GAM_check = 1
    End Sub

    Private Sub cb_GAM2_Checked(sender As Object, e As RoutedEventArgs) Handles cb_GAM2.Checked
        cb_GAM1.IsChecked = False
        cb_GAM3.IsChecked = False
        GAM_check = 2
    End Sub

    Private Sub cb_GAM3_Checked(sender As Object, e As RoutedEventArgs) Handles cb_GAM3.Checked
        cb_GAM1.IsChecked = False
        cb_GAM2.IsChecked = False
        GAM_check = 3
    End Sub

    Private Sub cb_GAM1_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_GAM1.Unchecked
        GAM_check = 0
    End Sub

    Private Sub cb_GAM2_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_GAM2.Unchecked
        GAM_check = 0
    End Sub
    Private Sub cb_GAM3_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_GAM3.Unchecked
        GAM_check = 0
    End Sub


    'Now starts GOS
    '========================================= GOS =====================================================================
    Private Sub cb_GOS1_Checked(sender As Object, e As RoutedEventArgs) Handles cb_GOS1.Checked
        cb_GOS2.IsChecked = False
        cb_GOS3.IsChecked = False
        GOS_check = 1
    End Sub

    Private Sub cb_GOS2_Checked(sender As Object, e As RoutedEventArgs) Handles cb_GOS2.Checked
        cb_GOS1.IsChecked = False
        cb_GOS3.IsChecked = False
        GOS_check = 2
    End Sub

    Private Sub cb_GOS3_Checked(sender As Object, e As RoutedEventArgs) Handles cb_GOS3.Checked
        cb_GOS1.IsChecked = False
        cb_GOS2.IsChecked = False
        GOS_check = 3
    End Sub

    Private Sub cb_GOS1_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_GOS1.Unchecked
        GOS_check = 0
    End Sub

    Private Sub cb_GOS2_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_GOS2.Unchecked
        GOS_check = 0
    End Sub
    Private Sub cb_GOS3_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_GOS3.Unchecked
        GOS_check = 0
    End Sub



    'Now starts GSD
    '========================================= GSD =====================================================================
    Private Sub cb_GSD1_Checked(sender As Object, e As RoutedEventArgs) Handles cb_GSD1.Checked
        cb_GSD2.IsChecked = False
        cb_GSD3.IsChecked = False
        GSD_check = 1
    End Sub

    Private Sub cb_GSD2_Checked(sender As Object, e As RoutedEventArgs) Handles cb_GSD2.Checked
        cb_GSD1.IsChecked = False
        cb_GSD3.IsChecked = False
        GSD_check = 2
    End Sub

    Private Sub cb_GSD3_Checked(sender As Object, e As RoutedEventArgs) Handles cb_GSD3.Checked
        cb_GSD1.IsChecked = False
        cb_GSD2.IsChecked = False
        GSD_check = 3
    End Sub

    Private Sub cb_GSD1_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_GSD1.Unchecked
        GSD_check = 0
    End Sub

    Private Sub cb_GSD2_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_GSD2.Unchecked
        GSD_check = 0
    End Sub
    Private Sub cb_GSD3_Unchecked(sender As Object, e As RoutedEventArgs) Handles cb_GSD3.Unchecked
        GSD_check = 0
    End Sub

    Private Sub finishButton_Click(sender As Object, e As RoutedEventArgs) Handles finishButton.Click

        'Nullifying the error_counter variable for fresh counting 
        error_counter = 0

        '================  processing of all data about KAM begins here  ===========================================
        If ((KAM_grid.IsEnabled = True) And (KAM_check = 0)) Then
            messageBox("At least one checkbox of KAM options must be checked")
        ElseIf ((KAM_grid.IsEnabled = True) And (KAM_check = 3)) Then
            If (IsNumeric(tb_kam1.Text) And IsNumeric(tb_kam2.Text)) Then
                If ((CDbl(tb_kam1.Text) > 0 And CDbl(tb_kam1.Text) < 100)) Then
                    If ((CInt(tb_kam2.Text) > 0) And (CInt(tb_kam2.Text) <= bins)) Then
                        kam_tb1 = tb_kam1.Text
                        kam_tb2 = tb_kam2.Text
                    Else
                        messageBox("Total no of bins for KAM is " + bins.ToString + ", 2nd input for KAM-option 3 is not valid")
                    End If
                Else
                    messageBox("1st input for KAM-option 3 is not valid")
                End If
            Else
                messageBox("The inputs for KAM-option 3 is not numeric")
            End If
        End If


        '================  processing of all data about GAM begins here  ===========================================
        If ((GAM_grid.IsEnabled = True) And (GAM_check = 0)) Then
            messageBox("At least one box of GAM options must be checked")
        ElseIf ((GAM_grid.IsEnabled = True) And (GAM_check = 3)) Then
            If (IsNumeric(tb_gam1.Text) And IsNumeric(tb_gam2.Text)) Then
                If ((CDbl(tb_gam1.Text) > 0 And CDbl(tb_gam1.Text) < 100)) Then
                    If ((CInt(tb_gam2.Text) > 0) And (CInt(tb_gam2.Text) <= bins)) Then
                        gam_tb1 = tb_gam1.Text
                        gam_tb2 = tb_gam2.Text
                    Else
                        messageBox("Total no of bins for GAM is " + bins.ToString + ", 2nd input for GAM-option 3 is not valid")
                    End If
                Else
                    messageBox("1st input for GAM-option 3 is not valid")
                End If
            Else
                messageBox("The inputs for GAM-option 3 is not numeric")
            End If
        End If


        '================  processing of all data about GOS begins here  ===========================================
        If ((GOS_grid.IsEnabled = True) And (GOS_check = 0)) Then
            messageBox("At least one box of GOS options must be checked")
        ElseIf ((GOS_grid.IsEnabled = True) And (GOS_check = 3)) Then
            If (IsNumeric(tb_gos1.Text) And IsNumeric(tb_gos2.Text)) Then
                If ((CDbl(tb_gos1.Text) > 0 And CDbl(tb_gos1.Text) < 100)) Then
                    If ((CInt(tb_gos2.Text) > 0) And (CInt(tb_gos2.Text) <= bins)) Then
                        gos_tb1 = tb_gos1.Text
                        gos_tb2 = tb_gos2.Text
                    Else
                        messageBox("Total no of bins for GOS is " + bins.ToString + ", 2nd input for GOS-option 3 is not valid")
                    End If
                Else
                    messageBox("1st input for GOS-option 3 is not valid")
                End If
            Else
                messageBox("The inputs for GOS-option 3 is not numeric")
            End If
        End If

        '================  processing of all data about GSD begins here  ===========================================
        If ((GSD_grid.IsEnabled = True) And (GSD_check = 0)) Then
            messageBox("At least one box of GSD options must be checked")
        ElseIf ((GSD_grid.IsEnabled = True) And (GSD_check = 3)) Then
            If (IsNumeric(tb_gsd1.Text) And IsNumeric(tb_gsd2.Text)) Then
                If ((CDbl(tb_gsd1.Text) > 0 And CDbl(tb_gsd1.Text) < 100)) Then
                    If ((CInt(tb_gsd2.Text) > 0) And (CInt(tb_gsd2.Text) <= bins)) Then
                        gsd_tb1 = tb_gsd1.Text
                        gsd_tb2 = tb_gsd2.Text
                    Else
                        messageBox("Total no of bins for GSD is " + bins.ToString + ", 2nd input for GSD-option 3 is not valid")
                    End If
                Else
                    messageBox("1st input for GSD-option 3 is not valid")
                End If
            Else
                messageBox("The inputs for GSD-option 3 is not numeric")
            End If
        End If

        If (error_counter = 0) Then

            Dim file As System.IO.StreamWriter
            file = My.Computer.FileSystem.OpenTextFileWriter("response.txt", True, System.Text.Encoding.ASCII)  'True appens the content

            file.WriteLine(KAM_check.ToString + " " + kam_tb1 + " " + kam_tb2 + " " + "0" + " " + "0")
            file.WriteLine(GAM_check.ToString + " " + gam_tb1 + " " + gam_tb2 + " " + "0" + " " + "0")
            file.WriteLine(GOS_check.ToString + " " + gos_tb1 + " " + gos_tb2 + " " + "0" + " " + "0")
            file.WriteLine(GSD_check.ToString + " " + gsd_tb1 + " " + gsd_tb2 + " " + "0" + " " + "0")
            file.Close()

            MsgBox("All values have been successfully taken" + Environment.NewLine _
                       + "This interface will close now!", MsgBoxStyle.Information)
            'Process.Start("C:\Users\Anil Kumar\Documents\Visual Studio 2013\Projects\CandC++\Release\CandC++.exe")
            'Process.Start("CandC++.exe")
            Try
                Process.Start("Main Application.exe")
            Catch ex As Exception
                MsgBox("Error in opening main application file:" & Environment.NewLine & ex.Message, MsgBoxStyle.Critical)
            End Try
            'Process.Start("C:\Users\Anil Kumar\Documents\Visual Studio 2013\Projects\CandC++\Release\CandC++.exe")
            'Process.Start("CandC++.exe")
            'Application.Current.Shutdown()
            Application.Current.Shutdown()
        End If


    End Sub

    Public Sub messageBox(message As String)
        MsgBox(message + Environment.NewLine + "Please modify the input to proceed", MsgBoxStyle.Information)
        error_counter += 1
    End Sub
End Class
