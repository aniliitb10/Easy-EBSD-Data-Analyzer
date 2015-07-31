'**************************************************************************************
'Easy EBSD Data Analyzer

'*	Author: Anil Kumar
'*	email: aniliitb10@gmail.com
'*	file: welcome.xaml.vb
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


Imports Microsoft.Win32

Class welcome
    Private error_counter As Integer = 0
    Private str As String = "(Your current .NET Framework Version: "

    Sub New()

        ' This call is required by the designer.
        InitializeComponent()
        welcomeTextBlock.Text = Get45or451FromRegistry()
        ' Add any initialization after the InitializeComponent() call.

    End Sub

    Private Sub welcomeButton_Click(sender As Object, e As RoutedEventArgs) Handles welcomeButton.Click
        If error_counter = 0 Then
            Dim browsePage As New browse_page()
            Me.NavigationService.Navigate(browsePage)
        Else
            MsgBox(".NET Framework version 4.5 or later is not detected." + Environment.NewLine +
            "Please update your .NET Framework", MsgBoxStyle.Information)
        End If

    End Sub

	
    Private Function Get45or451FromRegistry() As String

        Using ndpKey As RegistryKey = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine,
                    RegistryView.Registry32).OpenSubKey("SOFTWARE\Microsoft\NET Framework Setup\NDP\v4\Full\")

            If ndpKey IsNot Nothing AndAlso ndpKey.GetValue("Release") IsNot Nothing Then
                str += CheckFor45DotVersion(CInt(ndpKey.GetValue("Release")))
            Else
                str += "No 4.5 or later version detected, you may encounter some unexpected problem!)"
                error_counter += 1
            End If
        End Using

        Return str
    End Function

    Private Function CheckFor45DotVersion(releaseKey As Integer) As String
        If releaseKey >= 393273 Then
            Return "4.6 RC or later. You are good to go!)"
        End If
        If releaseKey >= 379893 Then
            Return "4.5.2 or later. You are good to go!)"
        End If
        If releaseKey >= 378675 Then
            Return "4.5.1 or later. You are good to go!)"
        End If
        If releaseKey >= 378389 Then
            Return "4.5 or later. You are good to go!)"
        End If
        ' This line should never execute. A non-null release key should mean 
        ' that 4.5 or later is installed. 
        Return "No 4.5 or later version detected, you may encounter some unexpected problem!)"
    End Function

End Class
