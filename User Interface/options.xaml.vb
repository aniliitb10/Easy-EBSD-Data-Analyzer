'**************************************************************************************
'Easy EBSD Data Analyzer

'*	Author: Anil Kumar
'*	email: aniliitb10@gmail.com
'*	file: options.xaml.vb
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


Class options

    Dim checkvalue1 As Integer = 0
    Dim checkvalue2 As Integer = 0
    Dim checkvalue3 As Integer = 0
    Dim checkvalue4 As Integer = 0
    Dim checkvalue5 As Integer = 0
    Dim checkvalue6 As Integer = 0
    Dim checkvalue7 As Integer = 0
    Dim index As Integer = 0
    'checking the checkboxes!
    Private Sub checkBox1_Checked(sender As Object, e As RoutedEventArgs) Handles checkBox1.Checked
        If checkBox1.IsChecked Then
            checkvalue1 = 1
            index += 1
        End If
    End Sub
    Private Sub checkBox2_Checked(sender As Object, e As RoutedEventArgs) Handles checkBox2.Checked
        If checkBox2.IsChecked Then
            checkvalue2 = 1
            index += 1
        End If
    End Sub
    Private Sub checkBox3_Checked(sender As Object, e As RoutedEventArgs) Handles checkBox3.Checked
        If checkBox3.IsChecked Then
            checkvalue3 = 1
            index += 1
        End If
    End Sub
    Private Sub checkBox4_Checked(sender As Object, e As RoutedEventArgs) Handles checkBox4.Checked
        If checkBox4.IsChecked Then
            checkvalue4 = 1
            index += 1
        End If
    End Sub
    Private Sub checkBox5_Checked(sender As Object, e As RoutedEventArgs) Handles checkBox5.Checked
        If checkBox5.IsChecked Then
            checkvalue5 = 1
            index += 1
        End If
    End Sub
    Private Sub checkBox6_Checked(sender As Object, e As RoutedEventArgs) Handles checkBox6.Checked
        If checkBox6.IsChecked Then
            checkvalue6 = 1
            index += 1
        End If
    End Sub

    Private Sub checkBox7_Checked(sender As Object, e As RoutedEventArgs) Handles checkBox7.Checked
        If checkBox7.IsChecked Then
            checkvalue7 = 1
            index += 1
        End If
    End Sub

    Private Sub checkBox1_Unchecked(sender As Object, e As RoutedEventArgs) Handles checkBox1.Unchecked
        checkvalue1 = 0
        index -= 1
    End Sub
    Private Sub checkBox2_Unchecked(sender As Object, e As RoutedEventArgs) Handles checkBox2.Unchecked
        checkvalue2 = 0
        index -= 1
    End Sub

    Private Sub checkBox3_Unchecked(sender As Object, e As RoutedEventArgs) Handles checkBox3.Unchecked
        checkvalue3 = 0
        index -= 1
    End Sub
    Private Sub checkBox4_Unchecked(sender As Object, e As RoutedEventArgs) Handles checkBox4.Unchecked
        checkvalue4 = 0
        index -= 1
    End Sub
    Private Sub checkBox5_Unchecked(sender As Object, e As RoutedEventArgs) Handles checkBox5.Unchecked
        checkvalue5 = 0
        index -= 1
    End Sub

    Private Sub checkBox6_Unchecked(sender As Object, e As RoutedEventArgs) Handles checkBox6.Unchecked
        checkvalue6 = 0
        index -= 1
    End Sub
    Private Sub checkBox7_Unchecked(sender As Object, e As RoutedEventArgs) Handles checkBox7.Unchecked
        checkvalue7 = 0
        index -= 1
    End Sub


    Private Sub optionNextButtton_Click(sender As Object, e As RoutedEventArgs) Handles optionNextButtton.Click
        If index < 1 Then
            MsgBox("At least one of the options must be checked", MsgBoxStyle.Information)

            'Process.Start("C:\Users\Anil Kumar\Documents\Visual Studio 2013\Projects\newUI\newUI\bin\Debug\newUI.exe")
            'Exit Sub

        Else
            '    Dim file_adress As System.IO.StreamWriter 'System.Text.ASCIIEncoding()
            '    file_adress = My.Computer.FileSystem.OpenTextFileWriter("check_response.txt", False, System.Text.Encoding.ASCII)
            '    'System.Text.Encoding.ASCII: took one day

            '    file_adress.WriteLine(checkvalue1.ToString() + " " + checkvalue2.ToString() + " " + checkvalue3.ToString() + " " + _
            '        checkvalue4.ToString() + " " + checkvalue5.ToString() + " " + checkvalue6.ToString() + " " + checkvalue7.ToString())
            '    file_adress.Close()


            Dim data_page As New Data()

            Me.NavigationService.Navigate(data_page)
            data_page.initialize(checkvalue1, checkvalue2, checkvalue3, checkvalue4, checkvalue5, checkvalue6, checkvalue7)
        End If

    End Sub

End Class
