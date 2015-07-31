'**************************************************************************************
'Easy EBSD Data Analyzer

'*	Author: Anil Kumar
'*	email: aniliitb10@gmail.com
'*	file: browse_page.xaml.vb
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


Class browse_page

    Dim filePathSelction As Integer = 0
    Dim corrected_filepath As String = ""

    Private Sub browseButton_Click(sender As Object, e As RoutedEventArgs) Handles browseButton.Click

       

        Dim myFileDialog As New Microsoft.Win32.OpenFileDialog()

        myFileDialog.InitialDirectory = "c:\"
        myFileDialog.Filter = "All files (*.*)| *.*" & "|Data Files (*.txt)|*.txt"
        myFileDialog.FilterIndex = 2
        myFileDialog.RestoreDirectory = True

        If myFileDialog.ShowDialog = True Then
            If Dir(myFileDialog.FileName) <> "" Then
                MsgBox("The file address has been taken", MsgBoxStyle.Information)
                filePathSelction = 1
            Else
                MsgBox("File not found, browse again to proceed", MsgBoxStyle.Critical)
            End If
        End If
        '"MsgBoxStyle.Information" provides msgbox sound and the icon

        browseTextBox.Text = myFileDialog.FileName


        For Each chars As Char In myFileDialog.FileName
            If chars = "\" Then
                corrected_filepath += "/"
            Else
                corrected_filepath += chars
            End If
        Next

        'writing the first file that has address and the no of lines to be processed!

        Dim file_adress As System.IO.StreamWriter 'System.Text.ASCIIEncoding()
        file_adress = My.Computer.FileSystem.OpenTextFileWriter("filepath.txt", False, System.Text.Encoding.ASCII)
        'System.Text.Encoding.ASCII: took one day!

        file_adress.WriteLine(corrected_filepath)
        file_adress.Close()

    End Sub



    Private Sub BrowseNextButton_Click(sender As Object, e As RoutedEventArgs) Handles BrowseNextButton.Click
        If filePathSelction = 0 Then
            MsgBox("You must select the data file to proceed", MsgBoxStyle.Information)
        ElseIf filePathSelction = 1 Then
            Dim calc_options As New options()
            Me.NavigationService.Navigate(calc_options)
        End If
    End Sub
End Class
